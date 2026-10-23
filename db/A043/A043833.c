/*
 * A043833  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 1 mod 5.
 * https://oeis.org/A043833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043833: a(n) = nth integer k such that the number of runs in the
   base‑4 representation of k is ≡ 1 mod 5. This implementation performs a
   simple brute‑force search. */

seq_t a(idx_t n)
{
    /* Offset is 1; return 0 for n == 0 (should not be requested). */
    if (n == 0)
        return 0;

    seq_t k = 1;           /* candidate integer */
    idx_t found = 0;       /* how many qualifying numbers found so far */

    while (1) {
        /* Count runs in base‑4 representation of k. */
        seq_t tmp = k;
        int runs = 0;
        int last = -1;      /* no last digit yet */

        while (tmp != 0) {
            int digit = (int)(tmp % 4);   /* 0..3 */
            if (digit != last) {
                runs++;
                last = digit;
            }
            tmp /= 4;
        }

        if ((runs % 5) == 1) { /* qualifies */
            found++;
            if (found == n)
                return k;
        }
        k++; /* next candidate */
    }
}
