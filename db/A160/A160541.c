/*
 * A160541  Number of odd-then-even runs to reach 1 from n under the modified
 *          "3x+1" map: x -> x/2 if x is even, x -> (3x+1)/2 if x is odd.
 * https://oeis.org/A160541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (number of odd → even transitions in the modified *
 *            Collatz sequence) + (n is even ? 1 : 0). This counts the
 *            odd‑then‑even * runs required to reach 1 under the map: x → x/2
 *            if x is even, * x → (3x+1)/2 if x is odd. The 2→1 step is
 *            omitted from the count
 * Range:     u8 n <= 26, u16 n <= 4126, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* convert index to the sequence type */
    seq_t count = 0;
    if (x == 1)
        return 0;                /* a(1) = 0 by definition */

    /* Scan the trajectory from x to 1 and count odd → even transitions. */
    seq_t y = x;
    while (y != 1) {
        if ((y & 1) != 0) {      /* y is odd */
            seq_t next = ((seq_t)3 * y + 1) >> 1;  /* (3y+1)/2 */
            if ((next & 1) == 0)   /* transition to an even number */
                count++;
            y = next;
        } else {                 /* y is even */
            y >>= 1;             /* divide by 2 */
        }
    }

    /* If the initial n is even, the run that ends at 2 is counted
     * but the 2→1 step is not. Add one for the initial even case. */
    if ((x & 1) == 0)
        count++;

    return count;
}
