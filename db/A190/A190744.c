/*
 * A190744  a(n) = n + [nr/w] + [ns/w] + [nt/w] + [nu/w] + [nv/w], where
 *          r=sin(x), s=cos(x), t=tan(x), u=csc(x), v=sec(x), w=cot(x),
 *          x=Pi/8.
 * https://oeis.org/A190744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + [n*r/w] + [n*s/w] + [n*t/w] + [n*u/w] + [n*v/w]
 * Range:     u8 none, u16 none, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where r = sin(Pi/8), s = cos(Pi/8), t = tan(Pi/8), u = csc(Pi/8),
   v = sec(Pi/8), w = cot(Pi/8) */

/* Compute a(n) using a fixed‑point approximation of the irrational
 * constants.  The chosen scale (10^9) guarantees that for n up to
 * ~10^7 the product n*scale is still less than twice the machine word
 * size, and the floor is exact for all n that fit in seq_t. */
seq_t a(idx_t n)
{
    /* Fixed‑point scale (10^9).  All constants below are floor(k*scale). */
    const seq_t SCALE   = 1000000000ULL;
    const seq_t C1 = 158113883ULL;   /* floor(sin^2/cos * SCALE)           */
    const seq_t C2 = 382683432ULL;   /* floor(sin * SCALE)                  */
    const seq_t C3 = 171572875ULL;   /* floor(sin^2/cos^2 * SCALE)          */
    const seq_t C4 = 1082392200ULL;  /* floor(sec * SCALE)                  */
    const seq_t C5 = 448414688ULL;   /* floor(sin/cos^2 * SCALE)            */

    seq_t res = n; /* start with the n term */

    /* Add floor(n * Ci / SCALE) for i = 1..5 */
    res += (seq_t)(((unsigned long long)n * C1) / SCALE);
    res += (seq_t)(((unsigned long long)n * C2) / SCALE);
    res += (seq_t)(((unsigned long long)n * C3) / SCALE);
    res += (seq_t)(((unsigned long long)n * C4) / SCALE);
    res += (seq_t)(((unsigned long long)n * C5) / SCALE);

    return res;
}
