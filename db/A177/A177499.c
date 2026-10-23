/*
 * A177499  Period 4: repeat [1, 16, 4, 16].
 * https://oeis.org/A177499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A177499: period‑4 repetition: 1, 16, 4, 16 (offset 0) */
seq_t a(idx_t n)
{
    /* Determine n mod 4 using a bitmask (4 is a power of two) */
    idx_t r = n & 3ULL;        /* r = n % 4 */

    switch (r) {
    case 0: return (seq_t)1;   /* a(0), a(4), a(8), ... */
    case 1: return (seq_t)16;  /* a(1), a(5), a(9), ... */
    case 2: return (seq_t)4;   /* a(2), a(6), a(10), ... */
    case 3: return (seq_t)16;  /* a(3), a(7), a(11), ... */
    }
    /* Should never reach here; but return a default to satisfy all paths */
    return (seq_t)0;
}
