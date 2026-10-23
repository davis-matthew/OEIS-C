/*
 * A018760  Divisors of 987.
 * https://oeis.org/A018760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 987 (sorted ascending) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, ignore n==0 */

    /* Precomputed divisor array for 987 */
    static seq_t divs[16];
    static idx_t divcnt = 0;

    if (divcnt == 0) {                  /* compute once */
        const idx_t target = 987;
        idx_t count = 0;
        for (idx_t i = 1; i <= target; ++i) {
            if (target % i == 0) {
                divs[count++] = (seq_t)i;
            }
        }
        divcnt = count;                  /* divcnt == 8 for 987 */
    }

    if (n <= divcnt) return divs[n - 1];
    return 0;                            /* undefined beyond last divisor */
}
