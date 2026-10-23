/*
 * A031253  Write 2n-1 in base 5 and juxtapose.
 * https://oeis.org/A031253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth digit of the string obtained by concatenating the base‑5
   representations of the odd numbers 1,3,5,… (most significant digit first). */

seq_t a(idx_t n)
{
    /* Offset starts at 1; if an out‑of‑range n is supplied, return 0. */
    if (n == 0) return 0;

    idx_t pos = 0;                         /* current position in string (1‑based) */

    for (idx_t odd = 1; ; odd += 2) {      /* iterate over odd numbers 1,3,5,… */
        /* Convert 'odd' to base‑5 digits (least significant first). */
        unsigned char digs[32];             /* 32 digits are more than enough for 64‑bit odds. */
        idx_t len = 0;
        idx_t tmp = odd;
        do {
            digs[len++] = tmp % 5;
            tmp /= 5;
        } while (tmp != 0);

        /* Emit digits most significant first. */
        for (idx_t i = len; i > 0; --i) {
            ++pos;
            if (pos == n) {
                return (seq_t)digs[i - 1];
            }
        }
    }
}
