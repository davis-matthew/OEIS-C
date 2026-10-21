/*
 * A058635  a(n) = Fibonacci(2^n).
 * https://oeis.org/A058635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(2^n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes F(2^n) using the fast‑doubling recurrence:
   if (F(k), F(k+1)) are known, then
       F(2k)   = F(k) * (2*F(k+1) - F(k))
       F(2k+1) = F(k)*F(k) + F(k+1)*F(k+1)
   Starting with (F(1)=1, F(2)=1) and applying the rule n
   times yields F(2^n).  */

seq_t a(idx_t n) {
    /* initial pair (F(1), F(2)) */
    seq_t f     = 1;   /* F(k)   where k = 1 */
    seq_t f_next = 1;  /* F(k+1) where k = 1 */

    /* iterate n times: each iteration doubles the index */
    for (idx_t i = 0; i < n; ++i) {
        seq_t t        = (f_next << 1) - f;          /* 2*F(k+1) - F(k) */
        seq_t new_f    = f * t;                      /* F(2k)          */
        seq_t new_fnext = f * f + f_next * f_next;   /* F(2k+1)        */
        f     = new_f;
        f_next = new_fnext;
    }
    return f;   /* F(2^n) */
}
