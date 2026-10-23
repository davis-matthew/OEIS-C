/*
 * A174138  Numbers congruent to {5,6,7,8,9,15,16,17,18,19} mod 25.
 * https://oeis.org/A174138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174138: numbers congruent to {5,6,7,8,9,15,16,17,18,19} mod 25.
   Formula used:  a(n) = base[(n-1)%10] + 25*floor((n-1)/10),
   where base = {5,6,7,8,9,15,16,17,18,19}. */
seq_t a(idx_t n)
{
    /* Guard against n < 1 – behaviour is undefined in original OEIS entry. */
    if (n == 0) return (seq_t)0;

    static const seq_t base[10] = {5, 6, 7, 8, 9, 15, 16, 17, 18, 19};

    /* Convert to 0‑based indexing inside the period. */
    idx_t pos = (n - 1) % 10;
    /* Count how many full periods of length 10 have passed. */
    idx_t k   = (n - 1) / 10;

    /* Compute the value.  All arithmetic is done in seq_t. */
    return base[pos] + (seq_t)25 * (seq_t)k;
}
