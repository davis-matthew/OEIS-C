/*
 * A018321  Divisors of 180.
 * https://oeis.org/A018321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 180, ordered ascending (a(1)=1, a(2)=2, …, a(18)=180) */

static const seq_t divisors[18] = {
    1, 2, 3, 4, 5, 6, 9, 10,
    12, 15, 18, 20, 30, 36, 45, 60,
    90, 180
};

seq_t a(idx_t n)
{
    /* OEIS uses 1‑based indexing; 0 or >18 calls return 0 */
    if (n == 0 || n > 18) return 0;
    return divisors[n - 1];
}
