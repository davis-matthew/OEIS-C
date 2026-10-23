/*
 * A023700  Numbers with exactly 2 2's in their ternary expansion.
 * https://oeis.org/A023700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the nth term of OEIS A023700.
   A023700: numbers whose ternary (base‑3) representation
   contains exactly two digits equal to 2.  The function
   evaluates the nth element by binary search and digit
   counting – no table lookup is used. */
#include <stddef.h>

/* Count how many integers <= X have exactly two 2's in their
   ternary representation. */
static idx_t count_leq(seq_t X)
{
    /* Extract ternary digits of X (least significant first). */
    unsigned char digits[128];
    idx_t len = 0;
    seq_t tmp = X;
    while (tmp != (seq_t)0) {
        digits[len++] = (unsigned char)(tmp % (seq_t)3);
        tmp /= (seq_t)3;
    }
    if (len == 0) return 0;           /* X == 0 */

    idx_t remaining_twos = 2;         /* still need two 2's */
    idx_t count = 0;

    /* Process digits from most significant to least. */
    for (idx_t i = 0; i < len; ++i) {
        idx_t pos = len - 1 - i;
        unsigned char cur = digits[pos];
        idx_t remaining_positions = len - 1 - i;

        /* Count numbers where current digit is < cur. */
        for (unsigned char val = 0; val < cur; ++val) {
            idx_t twos_left = remaining_twos - (val == 2 ? 1 : 0);
            if (twos_left < 0 || twos_left > remaining_positions)
                continue;

            /* Binomial coefficient C(remaining_positions, twos_left). */
            idx_t k = twos_left;
            if (k > remaining_positions - k)
                k = remaining_positions - k;
            idx_t res = 1;
            for (idx_t j = 1; j <= k; ++j)
                res = res * (remaining_positions - k + j) / j;

            /* Remaining digits (not 2) can be 0 or 1. */
            idx_t pow2 = (idx_t)1 << (remaining_positions - twos_left);

            count += res * pow2;
        }

        /* Update the number of 2's still needed. */
        if (cur == 2) {
            if (remaining_twos == 0)          /* already used two 2's */
                return count;                 /* no further contributions */
            --remaining_twos;
        }
    }

    /* If X itself satisfies the condition, include it. */
    if (remaining_twos == 0)
        ++count;

    return count;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Find the exponent i such that the cumulative count of
       numbers with <= i positions reaches at least n. */
    idx_t i = 1;
    idx_t cumulative = 0;
    while (1) {
        idx_t group_size = i * ((idx_t)1 << (i - 1));
        cumulative += group_size;
        if (cumulative >= n)
            break;
        ++i;
    }

    /* Pre‑compute powers of 3 up to exponent i+1 for binary search bounds. */
    idx_t max_exp = i + 1;
    seq_t *pow3 = (seq_t *)malloc((size_t)(max_exp + 1) * sizeof(seq_t));
    pow3[0] = (seq_t)1;
    for (idx_t k = 1; k <= max_exp; ++k)
        pow3[k] = pow3[k - 1] * (seq_t)3;

    /* Binary search for the nth value. */
    seq_t low = (seq_t)0;
    seq_t high = pow3[max_exp];            /* high is definitely large enough */

    while (low < high) {
        seq_t mid = (low + high) / (seq_t)2;
        if (count_leq(mid) < n)
            low = mid + (seq_t)1;
        else
            high = mid;
    }

    free(pow3);
    return low;
}
