/*
 * A203823  Number of (n+1)X6 0..3 arrays with no 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the order‑6 linear recurrence a(n) = 303*a(n-1) –
 *            8127*a(n-2) – 394891*a(n-3) + 4091304*a(n-4) + 21176820*a(n-5) –
 *            92236816*a(n-6), with a(1)..a(6) = 944784, 252047376,
 *            67240638864, 17968205387664, 4802315365587600,
 *            1283572086126147216. This recurrence is valid for all n ≥ 7
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 6, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First six terms – required to start the recurrence. */
    static const seq_t init[6] = {
        (seq_t)944784ULL,
        (seq_t)252047376ULL,
        (seq_t)67240638864ULL,
        (seq_t)17968205387664ULL,
        (seq_t)4802315365587600ULL,
        (seq_t)1283572086126147216ULL
    };

    if (n <= 6U) {
        /* n is unsigned; n-1 is valid because n>=1. */
        return init[n - 1U];
    }

    /* Sliding window of the last six values:
       prev[0] = a(k-6), prev[1] = a(k-5), …, prev[5] = a(k-1)   */
    seq_t prev[6];
    for (idx_t i = 0; i < 6U; ++i) {
        prev[i] = init[i];
    }

    seq_t next;
    for (idx_t k = 7U; k <= n; ++k) {
        /* Compute a(k) from the recurrence. */
        next = (seq_t)303   * prev[5]
             - (seq_t)8127  * prev[4]
             - (seq_t)394891 * prev[3]
             + (seq_t)4091304 * prev[2]
             + (seq_t)21176820 * prev[1]
             - (seq_t)92236816 * prev[0];

        /* Shift window for the next iteration. */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = next;
    }

    return prev[5];
}
