/*
 * A101293  a(n) = Lucas(n!).
 * https://oeis.org/A101293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000032(A000142(n))
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Helper structure for 2x2 integer matrices */
typedef struct {
    seq_t a, b, c, d;   /* Matrix [[a, b], [c, d]] */
} mat;

static inline mat mul(mat x, mat y) {
    mat r;
    r.a = x.a * y.a + x.b * y.c;
    r.b = x.a * y.b + x.b * y.d;
    r.c = x.c * y.a + x.d * y.c;
    r.d = x.c * y.b + x.d * y.d;
    return r;
}

/* Compute Lucas(k) for arbitrary-precision or native integer k */
static inline seq_t lucas_from_factorial(seq_t k) {
    mat res = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 };   /* Identity matrix */
    mat base = { (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)0 };   /* Fibonacci matrix */

    seq_t exp = k;
    while (exp > 0) {
        if ((exp & (seq_t)1) != 0) {
            res = mul(res, base);
        }
        base = mul(base, base);
        exp >>= 1;
    }

    /* Given res = [[F(k+1), F(k)], [...]], Lucas(k) = 2*F(k+1) - F(k) */
    return (seq_t)2 * res.a - res.b;
}

/* Main sequence function */
seq_t a(idx_t n) {
#ifdef __cplusplus
    /* Memoize factorials and Lucas values across calls */
    static std::vector<seq_t> facts{ (seq_t)1 };          /* facts[0] = 0! = 1 */
    static std::vector<seq_t> lucasVals{ lucas_from_factorial((seq_t)1) }; /* a(0) = Lucas(1) = 1 */

    /* Extend cache if necessary */
    if (n >= facts.size()) {
        for (idx_t i = facts.size(); i <= n; ++i) {
            seq_t f = facts.back() * (seq_t)i;
            facts.push_back(f);
            lucasVals.push_back(lucas_from_factorial(f));
        }
    }
    return lucasVals[n];
#else
    /* No memoization in plain C: recompute factorial each call */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }
    return lucas_from_factorial(fact);
#endif
}
