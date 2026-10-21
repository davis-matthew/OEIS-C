/*
 * A212681  Number of (w,x,y,z) with all terms in {1,...,n} and |x-y|<|y-z|.
 * https://oeis.org/A212681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A212681
   Recurrence: a(0)=0, a(1)=0, a(2)=4, a(3)=24, a(4)=88, a(5)=230,
   a(6)=504, a(7)=966, and for n ≥ 8
       a(n) = 3*a(n-1) - a(n-2) - 5*a(n-3) + 5*a(n-4)
              + a(n-5) - 3*a(n-6) + a(n-7).
   The function computes the value by iterating this recurrence; no
   memoisation is used, so it works for any order of calls. */
seq_t a(idx_t n)
{
        /* first eight values of the sequence (offset 0) */
        static const seq_t base[8] = {
                (seq_t)0,
                (seq_t)0,
                (seq_t)4,
                (seq_t)24,
                (seq_t)88,
                (seq_t)230,
                (seq_t)504,
                (seq_t)966
        };

        if (n < 8) return base[n];

        /* a0 … a6 hold a[n‑1] … a[n‑7] respectively */
        seq_t a0 = base[7]; /* a(n-1) for n=8 */
        seq_t a1 = base[6];
        seq_t a2 = base[5];
        seq_t a3 = base[4];
        seq_t a4 = base[3];
        seq_t a5 = base[2];
        seq_t a6 = base[1];

        for (idx_t i = 8; i <= n; ++i) {
                seq_t next =
                    3 * a0 - a1 - 5 * a2 + 5 * a3 + a4 - 3 * a5 + a6;
                /* shift the sliding window for the next step */
                a6 = a5;
                a5 = a4;
                a4 = a3;
                a3 = a2;
                a2 = a1;
                a1 = a0;
                a0 = next;
        }
        return a0;
}
