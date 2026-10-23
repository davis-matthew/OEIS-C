/*
 * A188949  Values of y such that x^2 + y^2 = 13^n with x and y coprime and 0
 *          < x < y.
 * https://oeis.org/A188949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(|Re((2+3i)^n)|, |Im((2+3i)^n)|) where (2+3i)^n is the
 *            n‑th power of the Gaussian integer 2+3i
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* signed big integer (seq_t is unsigned in the build) */
typedef struct {
    seq_t abs;   /* absolute value, always non‑negative */
    int   sign;  /* +1 for >=0, -1 for <0  (zero is treated as +1) */
} sgn_big;

/* construct sgn_big from a positive seq_t */
static inline sgn_big sgn_from_uint(seq_t v) { return (sgn_big){v, 1}; }

/* multiply two signed big integers */
static inline sgn_big mul_sgn(sgn_big a, sgn_big b) {
    seq_t prod = a.abs * b.abs;          /* potential overflow allowed */
    int   sggn = a.sign * b.sign;
    return (sgn_big){prod, sggn};
}

/* compare abs values of two sgn_big, return 1 if a>b, 0 if equal, -1 if a<b */
static inline int cmp_abs(const sgn_big *a, const sgn_big *b) {
    if (a->abs > b->abs) return 1;
    if (a->abs < b->abs) return -1;
    return 0;
}

/* add two signed big integers */
static inline sgn_big add_sgn(sgn_big a, sgn_big b) {
    if (a.sign == b.sign) {
        return (sgn_big){a.abs + b.abs, a.sign};
    } else {
        if (a.abs >= b.abs) {
            return (sgn_big){a.abs - b.abs, a.sign};
        } else {
            return (sgn_big){b.abs - a.abs, b.sign};
        }
    }
}

/* subtract b from a (i.e. a - b) */
static inline sgn_big sub_sgn(sgn_big a, sgn_big b) {
    b.sign = -b.sign;
    return add_sgn(a, b);
}

/* maximal absolute value of two sgn_big */
static inline seq_t max_abs_sgn(const sgn_big *a, const sgn_big *b) {
    return (cmp_abs(a, b) >= 0) ? a->abs : b->abs;
}

/* complex number over sgn_big */
typedef struct {
    sgn_big r;  /* real part */
    sgn_big i;  /* imaginary part */
} complex_sgn;

/* multiply two complex_sgn numbers */
static inline complex_sgn mul_complex(complex_sgn a, complex_sgn b) {
    complex_sgn c;

    /* real part: a.r*b.r - a.i*b.i */
    sgn_big ar_br = mul_sgn(a.r, b.r);
    sgn_big ai_bi = mul_sgn(a.i, b.i);
    c.r = sub_sgn(ar_br, ai_bi);

    /* imag part: a.r*b.i + a.i*b.r */
    sgn_big ar_bi = mul_sgn(a.r, b.i);
    sgn_big ai_br = mul_sgn(a.i, b.r);
    c.i = add_sgn(ar_bi, ai_br);

    return c;
}

/* compute a(idx_t n) */
seq_t a(idx_t n)
{
    /* base = 2 + 3i */
    complex_sgn base = { sgn_from_uint(2), sgn_from_uint(3) };
    /* result = 1 + 0i */
    complex_sgn result = { sgn_from_uint(1), sgn_from_uint(0) };

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            result = mul_complex(result, base);
        }
        base = mul_complex(base, base);
        exp >>= 1;
    }

    /* return max(|Re|, |Im|) */
    return max_abs_sgn(&result.r, &result.i);
}
