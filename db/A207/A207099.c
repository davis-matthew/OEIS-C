/*
 * A207099  Number of 0..7 arrays x(0..n-1) of n elements with each no smaller
 *          than the sum of its two previous neighbors modulo 8.
 * https://oeis.org/A207099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 8 a(2) = 36 For n ≥ 3: a(n) = sum_{prev,curr}
 *            f(prev,curr) · (M^(n‑2))[ (prev,curr) , * ] where f(prev,curr) =
 *            1 if curr ≥ prev else 0 The state (prev,curr) represents the
 *            last two elements of the array. The 64×64 transition matrix M is
 *            defined by M[(prev,curr)][(curr,next)] = 1 if next ≥...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

#define SZ 64              /* 8 × 8 states (prev, curr) */

static void build_matrix(seq_t M[SZ][SZ])
{
    for (idx_t i = 0; i < SZ; ++i)
        for (idx_t j = 0; j < SZ; ++j)
            M[i][j] = 0;

    for (idx_t prev = 0; prev < 8; ++prev)
        for (idx_t curr = 0; curr < 8; ++curr) {
            idx_t from = (prev << 3) | curr;
            idx_t sum  = (prev + curr) & 7;      /* (prev + curr) mod 8 */
            for (idx_t next = 0; next < 8; ++next) {
                if (next >= sum) {
                    idx_t to = (curr << 3) | next;
                    M[from][to] += 1;
                }
            }
        }
}

static void build_initial_vec(seq_t f[SZ])
{
    for (idx_t i = 0; i < SZ; ++i) {
        idx_t prev = i >> 3;          /* high 3 bits  */
        idx_t curr = i & 7;           /* low 3 bits   */
        f[i] = (curr >= prev) ? 1 : 0;
    }
}

static void mat_mul(const seq_t A[SZ][SZ], const seq_t B[SZ][SZ], seq_t C[SZ][SZ])
{
    seq_t tmp[SZ][SZ];
    for (idx_t i = 0; i < SZ; ++i)
        for (idx_t j = 0; j < SZ; ++j)
            tmp[i][j] = 0;

    for (idx_t i = 0; i < SZ; ++i)
        for (idx_t k = 0; k < SZ; ++k) {
            seq_t aik = A[i][k];
            if (aik == 0) continue;
            for (idx_t j = 0; j < SZ; ++j) {
                seq_t bkj = B[k][j];
                if (bkj == 0) continue;
                tmp[i][j] += aik * bkj;
            }
        }

    for (idx_t i = 0; i < SZ; ++i)
        for (idx_t j = 0; j < SZ; ++j)
            C[i][j] = tmp[i][j];
}

static void vec_mul(const seq_t v[SZ], const seq_t M[SZ][SZ], seq_t res[SZ])
{
    seq_t tmp[SZ];
    for (idx_t j = 0; j < SZ; ++j) tmp[j] = 0;

    for (idx_t i = 0; i < SZ; ++i) {
        seq_t vi = v[i];
        if (vi == 0) continue;
        for (idx_t j = 0; j < SZ; ++j) {
            seq_t mij = M[i][j];
            if (mij == 0) continue;
            tmp[j] += vi * mij;
        }
    }

    for (idx_t j = 0; j < SZ; ++j) res[j] = tmp[j];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined, but safe */

    if (n == 1) return (seq_t)8;  /* all single‑element arrays */

    /* Lazy static initialization */
    static seq_t M[SZ][SZ];
    static seq_t f[SZ];
    static bool init = false;
    if (!init) {
        build_matrix(M);
        build_initial_vec(f);
        init = true;
    }

    if (n == 2) {                /* sum of f over all states */
        seq_t total = 0;
        for (idx_t i = 0; i < SZ; ++i) total += f[i];
        return total;
    }

    /* n ≥ 3 : compute f · M^(n‑2) */
    idx_t exp = n - 2;
    seq_t v[SZ], temp_vec[SZ];
    for (idx_t i = 0; i < SZ; ++i) v[i] = f[i];

    seq_t curM[SZ][SZ], tmpM[SZ][SZ];
    for (idx_t i = 0; i < SZ; ++i)
        for (idx_t j = 0; j < SZ; ++j)
            curM[i][j] = M[i][j];

    while (exp > 0) {
        if (exp & (idx_t)1) {
            vec_mul(v, curM, temp_vec);
            for (idx_t i = 0; i < SZ; ++i) v[i] = temp_vec[i];
        }
        exp >>= 1;
        if (exp == 0) break;
        mat_mul(curM, curM, tmpM);
        for (idx_t i = 0; i < SZ; ++i)
            for (idx_t j = 0; j < SZ; ++j)
                curM[i][j] = tmpM[i][j];
    }

    /* sum all contributions */
    seq_t result = 0;
    for (idx_t i = 0; i < SZ; ++i) result += v[i];
    return result;
}
