/*
 * A032945  Numbers k whose base-10 representation Sum_{i=0..m} d(i)*10^(m-i)
 *          has d(i)=0 for all odd i. Here m is the position of the lead digit
 *          of k.
 * https://oeis.org/A032945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 799, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Algorithm: For each n (n >= 1) return the n-th smallest non‑negative integer
 * whose decimal digits have all odd‑indexed positions equal to 0 (index 0 is the
 * most‑significant digit).  Zero itself is the first element.  The numbers are
 * generated in ascending order by determining the digit length and the
 * appropriate digits, then inserting zeros after every odd position.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 0 is the first term */
    if (n == 1)
        return 0;

    /* index of the number among the positive terms (0‑based) */
    idx_t k = n - 2;

    /* pre‑compute powers of 10 up to the maximum needed (length <= 7) */
    idx_t pow10[10];
    pow10[0] = 1;
    for (int i = 1; i < 10; ++i)
        pow10[i] = pow10[i - 1] * (idx_t)10;

    /* find the digit length L such that the cumulative count up to L exceeds k */
    idx_t pref = 0;
    int L = 1;
    for (;; ++L) {
        int num_even = (L + 1) / 2;                /* number of positions that may be non‑zero */
        idx_t cnt = 9ULL * pow10[num_even - 1];   /* count of numbers of this length */
        if (pref + cnt > k)
            break;
        pref += cnt;
    }

    /* offset within numbers of length L */
    idx_t offset = k - pref;

    /* decompose offset into the digits for the even positions */
    int t = (L + 1) / 2;                /* number of even positions */
    idx_t first_digit = offset / pow10[t - 1] + 1;   /* leading non‑zero digit 1..9 */
    idx_t rem = offset % pow10[t - 1];

    seq_t digits[10];       /* t will never exceed 4 for n <= 10000 */
    digits[0] = (seq_t)first_digit;
    for (int i = 1; i < t; ++i) {
        int exp = t - 1 - i;
        digits[i] = (seq_t)((rem / pow10[exp]) % 10);
    }

    /* build the resulting number by inserting zeros after each odd‑indexed digit */
    seq_t result = 0;
    for (int i = 0; i < t; ++i) {
        result = result * (seq_t)10 + digits[i];
        /* add a zero unless we are at the final even position of an odd‑length number */
        if (!(L % 2 == 1 && i == t - 1))
            result = result * (seq_t)10;
    }

    return result;
}
