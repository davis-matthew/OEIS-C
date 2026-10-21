/*
 * A230710  Values of x such that x^2 + y^2 = 5^n with x and y coprime and 0 <
 *          x < y.
 * https://oeis.org/A230710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{|u(n)|, |v(n)|} where (u(n),v(n)) = (2+i)^n computed
 *            by the recurrence (u,v) → (2u−v, u+2v), starting at (u,v) =
 *            (2,1). This implementation uses signed big integers represented
 *            by an absolute value and a sign bit
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef struct { seq_t val; int sign; } Signed;

/* Normalise a signed value: zero is represented with positive sign. */
static inline Signed signed_norm(Signed s) {
    if (s.val == 0) s.sign = 1;
    return s;
}

/* Add two signed numbers: s = a + b. */
static inline Signed signed_add(Signed a, Signed b) {
    if (a.sign == b.sign) {
        return signed_norm((Signed){a.val + b.val, a.sign});
    }
    /* signs differ: subtract smaller magnitude from larger. */
    if (a.val >= b.val) {
        return signed_norm((Signed){a.val - b.val, a.sign});
    } else {
        return signed_norm((Signed){b.val - a.val, b.sign});
    }
}

/* Multiply signed number by 2. */
static inline Signed signed_mul2(Signed a) {
    return signed_norm((Signed){a.val * 2, a.sign});
}

/* Compute a(n). */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset starts at 1 */

    /* Initial (u,v) = (2,1). */
    Signed u = {(seq_t)2, 1};
    Signed v = {(seq_t)1, 1};

    for (idx_t i = 2; i <= n; ++i) {
        Signed old_u = u;

        Signed two_u = signed_mul2(u);
        Signed two_v = signed_mul2(v);

        /* new_u = 2*u - v  → 2*u + (-v) */
        Signed neg_v = {v.val, -v.sign};
        u = signed_add(two_u, neg_v);

        /* new_v = u + 2*v  → old_u + 2*v */
        v = signed_add(two_v, old_u);
    }

    /* a(n) is the minimum of the absolute values of u and v. */
    return (u.val < v.val) ? u.val : v.val;
}
