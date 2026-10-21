/*
 * A057538  Birthday set of order 5: numbers congruent to +-1 modulo 2, 3, 4
 *          and 5.
 * https://oeis.org/A057538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 60 * ((n-1)/8) + offsets[(n-1)%8] * Where offsets = {1,
 *            11, 19, 29, 31, 41, 49, 59}
 * Range:     u8 n <= 34, u16 n <= 8738, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* defined only for n >= 1, but guard anyway */

    static const seq_t offsets[8] = {
        (seq_t)1,  (seq_t)11, (seq_t)19,
        (seq_t)29, (seq_t)31, (seq_t)41,
        (seq_t)49, (seq_t)59
    };

    idx_t idx   = n - 1;          /* zero‑based index          */
    idx_t q     = idx / 8;        /* number of full 60‑steps   */
    idx_t r     = idx % 8;        /* which offset to add       */

    /* Use 128‑bit arithmetic to prevent overflow on intermediate
       multiplication when seq_t is only 64‑bit. */
    __int128 value = (__int128)60 * q + (__int128)offsets[r];

    return (seq_t)value;
}
