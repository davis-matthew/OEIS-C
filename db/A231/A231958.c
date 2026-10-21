/*
 * A231958  Numbers n dividing the Lucas sequence u(n) defined by u(i) =
 *          2*u(i-1) - 5*u(i-2) with initial conditions u(0)=0, u(1)=1.
 * https://oeis.org/A231958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer m such that m divides u(m),
   where u(0)=0, u(1)=1 and u(i)=2*u(i‑1)-5*u(i‑2). */

#include <stdlib.h>
#include <stdio.h>          /* for abort() if necessary */

/* --------------------------------------------------------------------
   2×2 matrix type and modular multiplication
   -------------------------------------------------------------------- */
typedef struct { seq_t a00, a01, a10, a11; } Mat;

static void mat_mul(const Mat *A, const Mat *B, idx_t mod, Mat *R)
{
    seq_t M = (seq_t)mod;
    R->a00 = (A->a00 * B->a00 + A->a01 * B->a10) % M;
    R->a01 = (A->a00 * B->a01 + A->a01 * B->a11) % M;
    R->a10 = (A->a10 * B->a00 + A->a11 * B->a10) % M;
    R->a11 = (A->a10 * B->a01 + A->a11 * B->a11) % M;
}

/* fast exponentiation of a 2×2 matrix modulo mod */
static void mat_pow(idx_t exp, idx_t mod, Mat *res)
{
    Mat R = {
        (seq_t)(1 % mod), 0, 0, (seq_t)(1 % mod)           /* identity */
    };
    Mat B;
    long long t = -5 % (long long)mod;
    if (t < 0) t += mod;
    B.a00 = (seq_t)(2 % mod);
    B.a01 = (seq_t)t;
    B.a10 = (seq_t)(1 % mod);
    B.a11 = 0;

    while (exp > 0) {
        if (exp & 1ULL) {
            Mat tmp;
            mat_mul(&R, &B, mod, &tmp);
            R = tmp;
        }
        exp >>= 1ULL;
        if (exp) {
            Mat tmp;
            mat_mul(&B, &B, mod, &tmp);
            B = tmp;
        }
    }
    *res = R;
}

/* --------------------------------------------------------------------
   Dynamic array storing the found terms
   -------------------------------------------------------------------- */
static seq_t *seq = NULL;      /* array of f(o)ut values  */
static size_t seq_cap = 0;     /* allocated capacity      */
static size_t seq_len = 0;     /* number of stored terms   */
static idx_t last_checked = 1; /* last integer examined   */
static int seq_initialized = 0;/* init flag                */

static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed */
    seq = tmp;
    seq_cap = new_cap;
}

static void push_term(idx_t m)
{
    ensure_capacity(seq_len + 1);
    seq[seq_len++] = (seq_t)m;
}

/* --------------------------------------------------------------------
   Main function: return the n‑th term of the sequence
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!seq_initialized) {
        ensure_capacity(16);
        seq_len = 1;
        seq[0] = (seq_t)1;       /* a(1)=1 */
        last_checked = 1;
        seq_initialized = 1;
    }

    while (seq_len < n) {
        idx_t m = last_checked + 1;
        last_checked = m;

        /* Compute u(m) modulo m by matrix exponentiation */
        if (m == 1) {            /* already stored */
            continue;
        }
        Mat M;
        mat_pow(m - 1, m, &M);   /* M = [[2,-5],[1,0]]^(m-1) mod m */

        /* u(m) mod m is the (0,0) entry of M times u(1)=1 */
        if (M.a00 == 0) {
            push_term(m);
        }
    }

    return seq[n - 1];
}
