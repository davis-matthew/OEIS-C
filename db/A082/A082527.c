/*
 * A082527  Least k such that x(k)=0 where x(1)=n and
 *          x(k)=k^2*floor(x(k-1)/k^2).
 * https://oeis.org/A082527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A082527: a(n) = least k ≥ 1 such that, beginning with x(1)=n
 * and recursively defining x(k)=k²·⌊x(k−1)/k²⌋ for k≥2,
 * the first k for which x(k)=0 is returned.
 * (For n=0 we directly return 1 since x(1)=0.) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                     /* x(1)=0 ⇒ k=1 */

    seq_t x = (seq_t)n;                      /* current value of x(k) */
    for (uint64_t k = 2; ; ++k) {            /* k is at least 2 during the loop */
        seq_t k2 = (seq_t)k * (seq_t)k;      /* k^2, using seq_t to avoid overflow */
        seq_t t   = x / k2;                  /* ⌊x/k²⌋ */

        if (t == 0)
            return (seq_t)k;                  /* reached zero */

        x = k2 * t;                          /* new x value, unchanged if exact division */
    }
}
