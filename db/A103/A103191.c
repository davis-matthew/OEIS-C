/*
 * A103191  A sequence of triples arising from a matrix calculation, in
 *          particular let m = floor(n/3), then (a(3*m), a(3*m+1), a(3*m+2)) =
 *          M^(m*(m+1)/n) * (0, 1, 1) where M is the matrix [[1,0,1], [1,1,0],
 *          [-1,1,0]].
 * https://oeis.org/A103191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   let m = floor(n/3). Compute k = m*(m+1)/2. Then a(n) is the (n
 *            mod 3)-th component of M^k*(0,1,1) where M = [[1,0,1], [1,1,0],
 *            [-1,1,0]]
 * Range:     u8 n <= 14, u16 n <= 20, u32 n <= 29, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* signed wrapper for seq_t (may be unsigned) */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* helper struct */
typedef struct {
    seq_t mag;   /* magnitude (non‑negative) */
    bool neg;    /* true if value is negative */
} sseq;

static inline sseq sseq_from_int(int64_t v)
{
    sseq r;
    if (v < 0) {
        r.neg = true;
        r.mag = (seq_t)(-v);
    } else {
        r.neg = false;
        r.mag = (seq_t)v;
    }
    return r;
}

/* signed addition */
static inline sseq sseq_add(sseq a, sseq b)
{
    if (a.neg == b.neg) {
        a.mag = a.mag + b.mag;
        return a;
    } else {
        if (a.mag >= b.mag) {
            a.mag = a.mag - b.mag;
            return a;
        } else {
            b.mag = b.mag - a.mag;
            return b;
        }
    }
}

/* signed multiplication */
static inline sseq sseq_mul(sseq a, sseq b)
{
    sseq r;
    r.neg = a.neg ^ b.neg;
    r.mag = a.mag * b.mag;
    return r;
}

/* matrix multiplication (3x3) */
static void mat_mul(const sseq a[3][3], const sseq b[3][3], sseq res[3][3])
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            sseq sum = sseq_from_int(0);
            for (int k = 0; k < 3; k++) {
                sseq prod = sseq_mul(a[i][k], b[k][j]);
                sum = sseq_add(sum, prod);
            }
            res[i][j] = sum;
        }
    }
}

/* binary exponentiation of a matrix */
static void mat_pow(sseq result[3][3], idx_t exp, const sseq base[3][3])
{
    /* identity matrix */
    sseq res[3][3];
    sseq cur[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            res[i][j] = (i == j) ? sseq_from_int(1) : sseq_from_int(0);
            cur[i][j] = base[i][j];
        }
    }

    while (exp > 0) {
        if (exp & 1) {
            sseq tmp[3][3];
            mat_mul(res, cur, tmp);
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    res[i][j] = tmp[i][j];
        }
        exp >>= 1;
        if (exp == 0) break;
        sseq tmp[3][3];
        mat_mul(cur, cur, tmp);
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                cur[i][j] = tmp[i][j];
    }

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            result[i][j] = res[i][j];
}

/* main sequence function */
seq_t a(idx_t n)
{
    idx_t m = n / 3;
    seq_t km = (seq_t)m * ((seq_t)m + 1) / 2;   /* as required */
    idx_t k = (idx_t)km;                         /* fits into idx_t because n <= 10000 */

    /* base matrix M */
    const sseq M[3][3] = {
        { sseq_from_int(1), sseq_from_int(0), sseq_from_int(1) },
        { sseq_from_int(1), sseq_from_int(1), sseq_from_int(0) },
        { sseq_from_int(-1), sseq_from_int(1), sseq_from_int(0) }
    };

    sseq P[3][3];
    mat_pow(P, k, M);

    /* compute required components */
    sseq a0 = sseq_add(P[0][1], P[0][2]);
    sseq a1 = sseq_add(P[1][1], P[1][2]);
    sseq a2 = sseq_add(P[2][1], P[2][2]);

    /* result is always non‑negative */
    seq_t result;
    switch (n % 3) {
        case 0: result = a0.neg ? (seq_t)0 : a0.mag; break;
        case 1: result = a1.neg ? (seq_t)0 : a1.mag; break;
        case 2: result = a2.neg ? (seq_t)0 : a2.mag; break;
        default: result = (seq_t)0; /* unreachable */
    }
    return result;
}
