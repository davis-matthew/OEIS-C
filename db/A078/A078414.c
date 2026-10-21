/*
 * A078414  a(n) = (a(n-1)+a(n-2))/7^k, where 7^k is the highest power of 7
 *          dividing a(n-1)+a(n-2).
 * https://oeis.org/A078414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n-1)+a(n-2)) divided by the largest power of 7
 *            dividing a(n-1)+a(n-2); a(1)=1, a(2)=1
 * Range:     u8 n <= 19, u16 n <= 90, u32 n <= 133, u64 n <= 255,
 *            u128 n <= 513, big any n
 * Helpers:   divide_by_max_power_of_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divide_by_max_power_of_7.h"
static seq_t seq[20001];       /* indices 0 … 20000, a[0] unused */
static idx_t seq_len = 0;      /* highest index that has been computed */

/* Return the n‑th term of OEIS A078414 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1 */

    /* Initialise the base cases on first use. */
    if (seq_len == 0) {
        seq[1] = 1;
        seq[2] = 1;
        seq_len = 2;
    }

    /* If the requested term is already known, return it. */
    if (n <= seq_len)
        return seq[n];

    /* Compute all missing terms up to n. */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t sum = seq[i - 1] + seq[i - 2];
        seq[i]    = divide_by_max_power_of_7(sum);
    }
    seq_len = n;                   /* now a[1..n] are valid */
    return seq[n];
}
