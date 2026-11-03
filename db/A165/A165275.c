/*
 * A165275  Table read by antidiagonals: T(n, k) is the k-th number with n-1
 *          odd-power summands in its base 2 representation.
 * https://oeis.org/A165275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A165275 – Table read by antidiagonals.
 * a(n) = k-th element of the antidiagonal enumeration of the array
 * T(n,k) = the k-th positive integer with n‑1 odd‑power summands in its
 *          base‑2 representation.
 * The implementation uses combinatorial ranking of binary strings
 * with a fixed number of odd positions set.
 * No external data structures or huge tables are required – the
 * algorithm runs in O(n·r) time where r ≤ 200 for n ≤ 10000.
 */

#include <stdbool.h>
#include <stdint.h>

/* Count the number of binary strings of length L that
 * have exactly r odd‑position bits set (the most
 * significant bit is always 1).  The result is capped
 * at 'limit'; if the true count exceeds the limit,
 * the routine returns limit+1.
 */
static inline seq_t count_len_with_limit(idx_t L, idx_t r, seq_t limit) {
    if (L == 0) return 0;

    idx_t msb_odd = ((L - 1) & 1);          /* is the most significant bit at an odd position? */
    if (r < msb_odd) return 0;              /* impossible if not enough odd bits required */
    idx_t needed = r - msb_odd;             /* odd bits still required below the MSB */

    /* Count odd and even positions among the remaining L‑1 bits */
    idx_t odd_rem  = (L - 1) / 2;           /* number of odd indices in 0..L‑2 */
    if (needed > odd_rem) return 0;         /* cannot satisfy the required odd bits */
    idx_t even_rem = (L - 1) - odd_rem;     /* count of remaining even indices */

    /* Compute C(odd_rem, needed) */
    seq_t bin = 1;
    for (idx_t i = 1; i <= needed; ++i) {
        seq_t num = odd_rem - needed + i;
        bin = (bin * num) / i;
        if (bin > limit) return limit + 1;
    }

    /* 2^even_rem possibilities for the even positions */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < even_rem; ++i) {
        if (pow2 > limit / 2) return limit + 1;
        pow2 <<= 1;
    }

    seq_t tot = bin * pow2;
    return (tot > limit) ? limit + 1 : tot;
}

/* Count the number of binary strings of length pos+1 that
 * have exactly rem odd‑position bits set, assuming the
 * most significant bit (at position pos) is forced to 0.
 * The result is capped at 'limit'.
 */
static inline seq_t count_lower_with_limit(idx_t pos, idx_t rem, seq_t limit) {
    if (pos == (idx_t)-1) return (rem == 0) ? 1 : 0;
    if (rem > pos + 1) return 0;           /* impossible when we would need more odd bits than positions */

    idx_t odd_positions  = (pos + 1) / 2;   /* odd indices in 0..pos */
    idx_t even_positions = (pos + 1) - odd_positions;

    if (rem > odd_positions) return 0;     /* cannot fill required odd bits */

    seq_t bin = 1;
    for (idx_t i = 1; i <= rem; ++i) {
        seq_t num = odd_positions - rem + i;
        bin = (bin * num) / i;
        if (bin > limit) return limit + 1;
    }

    seq_t pow2 = 1;
    for (idx_t i = 0; i < even_positions; ++i) {
        if (pow2 > limit / 2) return limit + 1;
        pow2 <<= 1;
    }

    seq_t tot = bin * pow2;
    return (tot > limit) ? limit + 1 : tot;
}

/* Given the 0‑based rank (column index) within row r,
 * return the corresponding integer.  r ≥ 0, rank ≥ 0.
 */
static seq_t unrank_row(seq_t rank, idx_t r) {
    seq_t cum = 0;
    idx_t L = 0;
    seq_t cnt_len = 0;

    /* Find the length L of the binary representation. */
    for (L = 1; ; ++L) {
        cnt_len = count_len_with_limit(L, r, rank);
        cum += cnt_len;
        if (cum > rank) break;
    }

    seq_t prevcum   = cum - cnt_len;
    seq_t rankwithin = rank - prevcum;

    /* Build the number bit by bit starting from the MSB downwards. */
    seq_t value = ((seq_t)1) << (L - 1);
    idx_t rem = r - (((L - 1) & 1) ? 1 : 0);  /* odd bits still needed below the MSB */

    for (long long pos = (long long)L - 2; pos >= 0; --pos) {
        seq_t count0;
        if (pos == 0) {
            count0 = (rem == 0) ? 1 : 0;     /* only one number for the last bit */
        } else {
            count0 = count_lower_with_limit((idx_t)pos - 1, rem, rankwithin);
        }

        if (count0 > rankwithin) {
            /* keep this bit 0 */
        } else {
            rankwithin -= count0;
            if ((pos & 1) == 1) rem -= 1;   /* a 1 at an odd position consumes one odd bit */
            value |= ((seq_t)1) << pos;     /* set the bit to 1 */
        }
    }

    return value;
}

/* Public entry point.  n is 1‑based (OEIS offset). */
seq_t a(idx_t n) {
    if (n == 0) return 0;                    /* defensive: OEIS offset starts at 1 */

    /* Convert 1‑based index to 0‑based linear index. */
    idx_t i = n - 1;

    /* Find diagonal sum s such that triangular(s) <= i < triangular(s+1). */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= i) {
        ++s;
    }

    /* Determine row (number of odd bits) and column (rank within that row). */
    idx_t d = i - (s * (s + 1)) / 2;
    idx_t r = d;              /* row index: r == number of odd powers */
    idx_t c = s - d;          /* column index: c == rank within row r */

    seq_t rank = (seq_t)c;
    return unrank_row(rank, r);
}
