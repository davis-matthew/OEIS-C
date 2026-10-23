/*
 * A034101  Numbers whose fractional part of square root starts with digit 5.
 * https://oeis.org/A034101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the nth integer x such that the fractional part of
 *            sqrt(x) begins with the digit 5. The algorithm enumerates the
 *            intervals [⌈(k+0.5)²⌉, ⌊(k+0.6)²⌋] for k = 0,1,2,… and counts
 *            the integers in them until the n‑th such integer is reached
 * Range:     u8 n <= 21, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* offset starts at 1 – undefined for n=0 */
    idx_t  count = 0;        /* numbers already found */
    idx_t  k     = 0;        /* current integer part of the square root */

    while (1) {
        /* compute ⌈(k+0.5)²⌉ and ⌊(k+0.6)²⌋ using 128‑bit arithmetic to avoid overflow */
        unsigned __int128 a128 = (unsigned __int128)k * 10 + 5;
        unsigned __int128 a2   = a128 * a128;
        seq_t lower = (seq_t)((a2 + 99) / 100);          /* ceil division */

        unsigned __int128 b128 = (unsigned __int128)k * 10 + 6;
        unsigned __int128 b2   = b128 * b128;
        seq_t upper = (seq_t)(b2 / 100);                 /* floor division */

        if (upper >= lower) {
            idx_t run = (idx_t)(upper - lower + 1);     /* length of current run */
            if (count + run >= n) {
                idx_t offset = n - count - 1;           /* zero‑based offset within the run */
                return lower + (seq_t)offset;
            }
            count += run;
        }
        k++;
    }
}
