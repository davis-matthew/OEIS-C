/*
 * A015580  Expansion of x/(1 - 9*x - 4*x^2).
 * https://oeis.org/A015580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + 4*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial two terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* 2x2 matrix structure:
         [m00 m01]
         [m10 m11]
       Stored in row-major order in a length‑4 array. */
    seq_t base[4] = { (seq_t)9, (seq_t)4, (seq_t)1, (seq_t)0 };      /* [[9,4],[1,0]] */
    seq_t res[4]  = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 };      /* identity matrix */

    idx_t exp = n - 1;   /* we need M^(n‑1) */

    while (exp) {
        if (exp & 1) {
            /* res = res * base */
            seq_t t0 = res[0]*base[0] + res[1]*base[2];
            seq_t t1 = res[0]*base[1] + res[1]*base[3];
            seq_t t2 = res[2]*base[0] + res[3]*base[2];
            seq_t t3 = res[2]*base[1] + res[3]*base[3];
            res[0] = t0; res[1] = t1; res[2] = t2; res[3] = t3;
        }
        /* base = base * base */
        seq_t t0 = base[0]*base[0] + base[1]*base[2];
        seq_t t1 = base[0]*base[1] + base[1]*base[3];
        seq_t t2 = base[2]*base[0] + base[3]*base[2];
        seq_t t3 = base[2]*base[1] + base[3]*base[3];
        base[0] = t0; base[1] = t1; base[2] = t2; base[3] = t3;

        exp >>= 1;
    }

    /* a(n) is the (0,0) entry of the resulting matrix */
    return res[0];
}
