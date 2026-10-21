/*
 * A073730  Concatenation of largest and smallest digits of n in decimal
 *          representation.
 * https://oeis.org/A073730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A054055(n)*10 + A054054(n) (largest digit * 10 + smallest
 *            digit)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest and smallest decimal digits of the index n */

seq_t a(idx_t n)
{
    /* Handle the off‑by‑one offset: a(1) corresponds to n == 1 */
    if (n == 0)
        return (seq_t)0;

    /* Work with an unsigned 64‑bit copy of the index */
    uint64_t x = (uint64_t)n;
    unsigned largest = 0;
    unsigned smallest = 9;

    /* Extract decimal digits */
    while (x > 0) {
        unsigned d = (unsigned)(x % 10ULL);
        if (d > largest)  largest = d;
        if (d < smallest) smallest = d;
        x /= 10ULL;
    }

    /* Concatenate largest and smallest digits: two‑digit number */
    return (seq_t)largest * (seq_t)10 + (seq_t)smallest;
}
