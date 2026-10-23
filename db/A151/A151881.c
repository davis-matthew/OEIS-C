/*
 * A151881  Sum (number of cycles)^2 over all n! permutations of [1..n].
 * https://oeis.org/A151881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^(n+1)*(Stirling1(n+1,2)-2*Stirling1(n+1,3))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n >= 1.  Uses only integer arithmetic. */

typedef struct {
    seq_t mag;  /* magnitude (absolute value) */
    int   sign; /* +1 or -1  (0 is treated as +1) */
} signed_int;

static inline signed_int s_zero(void) {
    return (signed_int){0, 1};
}

static inline signed_int s_make(seq_t mag, int sign) {
    if (mag == (seq_t)0) return s_zero();
    return (signed_int){mag, (sign > 0) ? 1 : -1};
}

static inline signed_int s_neg(const signed_int *a) {
    if (a->mag == (seq_t)0) return s_zero();
    return (signed_int){a->mag, -a->sign};
}

static inline signed_int s_add(const signed_int *a, const signed_int *b) {
    if (a->mag == (seq_t)0) return *b;
    if (b->mag == (seq_t)0) return *a;
    if (a->sign == b->sign) {
        return (signed_int){a->mag + b->mag, a->sign};
    } else {
        if (a->mag >= b->mag)
            return (signed_int){a->mag - b->mag, a->sign};
        else
            return (signed_int){b->mag - a->mag, b->sign};
    }
}

static inline signed_int s_sub(const signed_int *a, const signed_int *b) {
    signed_int negb = s_neg(b);
    return s_add(a, &negb);
}

static inline signed_int s_mul_small(const signed_int *a, idx_t mul) {
    if (a->mag == (seq_t)0) return s_zero();
    return (signed_int){a->mag * (seq_t)mul, a->sign};
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n=0; return 0 for safety */

    signed_int old[4];
    old[0] = s_make((seq_t)1, 1);     /* S(0,0)=1 */
    old[1] = s_zero();
    old[2] = s_zero();
    old[3] = s_zero();

    for (idx_t m = 1; m <= n + 1; ++m) {
        signed_int next[4] = {s_zero(), s_zero(), s_zero(), s_zero()};
        for (int k = 1; k <= 3; ++k) {
            signed_int t1 = old[k - 1];
            signed_int t2 = s_mul_small(&old[k], m - 1);
            signed_int t2n = s_neg(&t2);
            next[k] = s_add(&t1, &t2n);
        }
        for (int k = 0; k < 4; ++k) old[k] = next[k];
    }

    /* temp = S(n+1,2) - 2*S(n+1,3) */
    signed_int double_old3 = s_mul_small(&old[3], 2);
    signed_int temp = s_sub(&old[2], &double_old3);

    /* Multiply by (-1)^(n+1) */
    if (((n + 1) & 1) != 0) {
        temp = s_neg(&temp);
    }

    /* Result is guaranteed non‑negative; return magnitude */
    return temp.mag;
}
