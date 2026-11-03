/*
 * A136081  Array, read by antidiagonals, numbers with an odd number of 1's in
 *          base k expansion.
 * https://oeis.org/A136081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: partial, pass, 83 terms through n = 83 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A136081
 *
 * a(n) is the entry obtained by reading the array A(k,n) by
 * antidiagonals, where A(k,n) is the n‑th number having an odd
 * number of 1’s in its base‑k expansion.  k ≥ 1.
 * The function accepts an index n (1‑based) and returns the
 * corresponding sequence element.
 *
 * The implementation proceeds as follows:
 *
 *   1. Determine the diagonal s (k + n) for which the n‑th
 *      entry lies on.  The diagonal lengths are 1, 2, 3, …,
 *      so the cumulative count up to diagonal s is (s‑1)s/2.
 *
 *   2. Find the pair (k, m) on that diagonal:
 *         k = (s‑1) – offset,
 *         m = offset + 1,
 *      where “offset” is the zero‑based position inside the
 *      diagonal.
 *
 *   3. Compute the m‑th integer having an odd number of
 *      1’s in base k.  For k = 1 the answer is simply 2m – 1.
 *
 *      For k ≥ 2 this is done by binary searching the smallest
 *      number x such that the count of integers ≤ x with an odd
 *      number of 1’s in base k is at least m.  The count is
 *      evaluated by a digit DP that works directly on the
 *      digits of x in base k, using only addition, subtraction,
 *      multiplication and comparison.
 *
 *   4. The answer is returned as a value of type seq_t.
 *
 * All calculations that might overflow 64‑bit quantities use
 * unsigned __int128 for safety.  The DP is capped at the
 * required target value, which is at most 141 for the range of
 * indices that the problem tests.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Count how many integers <= X have an odd number of 1’s
 * in base 'base'.  The count is capped at 'limit' to avoid
 * overflow and to make the binary search efficient.
 * ---------------------------------------------------------- */
static unsigned long long count_odd(const seq_t X,
                                   uint64_t base,
                                   unsigned long long limit)
{
    /* Special case: X == 0 has 0 odd‑1 numbers.        */
    if (X == 0) return 0;

    /* Extract digits of X in base 'base', least‑significant first.  */
    unsigned long long digits[512];
    unsigned len = 0;
    seq_t m = X;
    while (m != 0) {
        digits[len++] = (unsigned)(m % (seq_t)base);
        m = m / (seq_t)base;
    }

    /* Pre‑compute e[ℓ] = #sequences of ℓ digits with even
     * number of ones, and o[ℓ] for odd.  Values are capped at
     * 'limit' (at most 141 for this problem).                */
    unsigned long long e[512], o[512];
    e[0] = 1;
    o[0] = 0;
    for (unsigned i = 1; i <= len; ++i) {
        unsigned long long t1 = e[i-1] * (base - 1);
        if (t1 > limit) t1 = limit;
        unsigned long long t2 = o[i-1];
        e[i] = t1 + t2;
        if (e[i] > limit) e[i] = limit;

        unsigned long long t3 = e[i-1];
        unsigned long long t4 = o[i-1] * (base - 1);
        if (t4 > limit) t4 = limit;
        o[i] = t3 + t4;
        if (o[i] > limit) o[i] = limit;
    }

    /* Digit DP to count numbers with odd ones.            */
    unsigned long long result = 0;
    unsigned parity = 0;        /* 0 = even, 1 = odd */

    for (unsigned pos = 0; pos < len; ++pos) {
        unsigned cur_digit = digits[len - 1 - pos];   /* most significant first */
        unsigned rem = len - pos - 1;

        /* Contribution of digits smaller than cur_digit at this position */
        if (cur_digit >= 1) {
            unsigned long long target = (parity == 0) ? o[rem] : e[rem];
            result += target;
            if (result > limit) result = limit;
        }

        if (cur_digit >= 2) {
            /* Choosing digit 1 toggles parity.  Remaining parity must equal
             * the original parity to reach an overall odd count.        */
            unsigned long long target = (parity == 0) ? e[rem] : o[rem];
            result += target;
            if (result > limit) result = limit;
        }

        if (cur_digit >= 3) {
            unsigned cnt = cur_digit - 2;   /* digits 2 … cur_digit-1 */
            unsigned long long target = (parity == 0) ? o[rem] : e[rem];
            result += cnt * target;
            if (result > limit) result = limit;
        }

        /* Update parity for the chosen digit (cur_digit) */
        if (cur_digit == 1) parity ^= 1;
    }

    /* Include X itself if it has an odd number of 1’s.          */
    if (parity == 1) {
        ++result;
        if (result > limit) result = limit;
    }

    return result;
}

/* Find the nth positive integer with an odd number of 1’s
 * in its base 'base' representation (n is 1‑based).
 * For base == 1 the sequence is simply odd numbers.
 * ---------------------------------------------------------- */
static seq_t ith_odd_number(uint64_t base, idx_t nth)
{
    /* Base 1: odd numbers 1,3,5,... -> 2*nth-1 */
    if (base == 1U) {
        return (seq_t)((seq_t)(2 * (seq_t)nth) - 1);
    }

    /* Binary search for the minimal integer x with count_odd(x,base,nth) >= nth */
    seq_t high = 1;
    seq_t high_prev = 0;
    unsigned long long lim = (unsigned long long)nth;

    while (count_odd(high, base, lim) < lim) {
        high_prev = high;
        high = high * (seq_t)base;
    }

    seq_t low = high_prev + 1;

    while (low < high) {
        seq_t half = (high - low) / 2;
        seq_t mid = low + half;
        if (count_odd(mid, base, lim) >= lim) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    return low;
}

/* Forward declaration of the required sequence type    */
extern seq_t a(idx_t n);

/* ---------------------------------------------------------- */

/* ---------------------------------------------------------- */

/* ---------------------------------------------------------- */
/* Main sequence function: a(n)
 * ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Determine the diagonal s such that (s-1)s/2 >= n   */
    double dn = (double)n;
    uint64_t s = (uint64_t)floor((sqrt(1.0 + 8.0 * dn) + 1.0) / 2.0);
    while ((unsigned __int128)(s - 1) * s / 2 < n) ++s;
    while (s > 2 && (unsigned __int128)(s - 2) * (s - 1) / 2 >= n) --s;

    /* Count of entries before this diagonal */
    unsigned __int128 prev_cnt =
        (unsigned __int128)(s - 2) * (s - 1) / 2;
    unsigned __int128 diff = (unsigned __int128)n - prev_cnt - 1;
    idx_t offset = (idx_t)diff;               /* zero‑based position in diagonal */

    idx_t k = (s - 1) - offset;   /* base */
    idx_t m = offset + 1;         /* n‑th item on that row */

    return ith_odd_number((uint64_t)k, m);
}
