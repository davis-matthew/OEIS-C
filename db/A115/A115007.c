/*
 * A115007  Row 3 of array in A114999.
 * https://oeis.org/A115007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let q = n / 6 (integer division) and r = n % 6. Let p[6] = {0,
 *            6, 16, 31, 50, 75}. Then a(n) = 103*q + p[r] + 28*(3*q*(q-1) +
 *            q*r). This reproduces the OEIS sequence A115007
 * Range:     u8 n <= 9, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Prefix sums of the base differences for remainders 0..5 */
    static const seq_t p[6] = {0, 6, 16, 31, 50, 75};

    idx_t q = n / 6;          /* number of full 6‑element blocks   */
    idx_t r = n % 6;          /* remainder within the current block */

    /* Core computation: all arithmetic performed in seq_t. */
    seq_t result = (seq_t)q * 103 + p[r];

    if (q > 0) {
        seq_t qm = (seq_t)q;
        /* Sum of floor((i-1)/6) for i = 1..n:
           3*q*(q-1) counts the full blocks, q*r accounts for the remaining r elements. */
        seq_t sum_floor = (seq_t)3 * qm * (qm - 1) + qm * (seq_t)r;
        result += (seq_t)28 * sum_floor;
    }

    return result;
}
