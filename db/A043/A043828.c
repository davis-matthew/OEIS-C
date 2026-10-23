/*
 * A043828  Number of runs in the base 4 representation of n is congruent to 0
 *          mod 4.
 * https://oeis.org/A043828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose number of runs in the base 4
 *            * representation is congruent to 0 (mod 4)
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;        /* undefined for n=0, but safe return */
    }
    seq_t target = (seq_t)n;
    seq_t count   = (seq_t)0;
    seq_t num     = (seq_t)0;

    for (;;) {
        num = num + (seq_t)1;                 /* current integer to test */

        /* Count runs in base‑4 representation of num */
        seq_t x     = num;
        seq_t prev  = x % (seq_t)4;
        x          = x / (seq_t)4;
        seq_t runs  = (seq_t)1;               /* at least one run */

        while (x > (seq_t)0) {
            seq_t cur = x % (seq_t)4;
            if (cur != prev) {
                runs = runs + (seq_t)1;
            }
            prev = cur;
            x    = x / (seq_t)4;
        }

        if ((runs % (seq_t)4) == (seq_t)0) {  /* runs ≡ 0 (mod 4) */
            count = count + (seq_t)1;
            if (count == target) {
                return num;
            }
        }
    }
}
