/*
 * A207109  Number of n X 6 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 72, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207109: Number of n×6 0/1 arrays avoiding
 *  horizontal 0 0 0 and 0 1 1 and vertical 0 0 1 and 1 1 0.
 *
 *  The sequence satisfies the linear recurrence
 *      a(n) = 7*a(n‑1) - 21*a(n‑2) + 35*a(n‑3) - 35*a(n‑4)
 *           + 21*a(n‑5) - 7*a(n‑6) + a(n‑7)   for n > 7
 *  with initial values
 *      a(1)=18, a(2)=324, a(3)=1504, a(4)=4534,
 *      a(5)=10898, a(6)=22714, a(7)=42874.
 *
 *  The implementation uses only integer arithmetic and a sliding
 *  7‑term window, so it runs in O(n) time and O(1) extra space.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n = 0 (undefined case, harmless). */
    if (n == 0) return (seq_t)0;

    /* Initial values for n = 1 … 7 (1‑based). */
    static const seq_t init[7] = {
        (seq_t)18,
        (seq_t)324,
        (seq_t)1504,
        (seq_t)4534,
        (seq_t)10898,
        (seq_t)22714,
        (seq_t)42874
    };

    if (n <= 7) return init[n - 1];

    /* Sliding window of the last seven computed terms.
     * w[0] = a(n-7), w[1] = a(n-6), … , w[6] = a(n-1). */
    seq_t w[7];
    for (idx_t i = 0; i < 7; ++i) w[i] = init[i];

    seq_t next;
    for (idx_t i = 8; i <= n; ++i) {
        /* Compute a(i) from a(i-1)…a(i-7) with non‑negative arithmetic. */
        seq_t sum = ((seq_t)7  * w[6]) + ((seq_t)35 * w[4]) +
                    ((seq_t)21 * w[2]) + w[0];
        seq_t sub = ((seq_t)21 * w[5]) + ((seq_t)35 * w[3]) +
                    ((seq_t)7  * w[1]);

        next = sum - sub;  /* sum >= sub for all terms in the sequence */

        /* Shift window: drop w[0], shift others left, insert new term. */
        w[0] = w[1]; w[1] = w[2]; w[2] = w[3];
        w[3] = w[4]; w[4] = w[5]; w[5] = w[6];
        w[6] = next;
    }

    return w[6];
}
