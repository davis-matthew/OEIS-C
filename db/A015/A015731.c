/*
 * A015731  Inverse of 1722nd cyclotomic polynomial.
 * https://oeis.org/A015731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   mul_by_xminus1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse of 1722nd cyclotomic polynomial.
   a(0) = 1,
   for n > 0: a(n) = - Σ_{j=1}^{min(n,480)} φ[j] * a(n-j),
   where Φ_1722(x) = Σ_{j=0}^{480} φ[j] x^j. */

/* required headers */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mul_by_xminus1.h"

/* constants from the factorisation 1722 = 2·3·7·41 */
#define PERIOD      1722
#define NUM_DIV     16
#define DEG_N       2256   /* degree of the numerator polynomial N */
#define DEG_D       1776   /* degree of the denominator polynomial D */
#define DEG_Q       480    /* degree of Φ_1722(x) */

/* divisor list of 1722 */
static const idx_t DIVISORS[NUM_DIV] = {
    1,   2,   3,   6,   7,   14,  21,  42,
    41,  82,  123, 246, 287, 574, 861, 1722
};

/* sign for each divisor : +1 for μ(n/d)= 1 (numerator), -1 for μ(n/d)= -1 (denominator) */
static const int8_t SIGNS[NUM_DIV] = {
    1,  -1,  -1,   1,  -1,   1,   1,  -1,
   -1,   1,   1,  -1,   1,  -1,  -1,   1
};

/* polynomial storage */
static seq_t N[DEG_N + 1];
static seq_t D[DEG_D + 1];
static seq_t Q[DEG_Q + 1];
static seq_t seq_arr[PERIOD];

/* initialisation of N, D, Q, and the sequence values */
static void init_sequence(void)
{
    /* build N and D as products of (x^d - 1) with appropriate signs */
    int degN = 0, degD = 0;
    N[0] = 1; for (int i = 1; i <= DEG_N; ++i) N[i] = 0;
    D[0] = 1; for (int i = 1; i <= DEG_D; ++i) D[i] = 0;
    seq_t TMP[DEG_N + 1];

    for (int k = 0; k < NUM_DIV; ++k) {
        int d = (int)DIVISORS[k];
        if (SIGNS[k] == 1) {                 /* μ =  1 → numerator */
            mul_by_xminus1(N, degN, d, TMP);
            degN += d;
            for (int i = 0; i <= degN; ++i) N[i] = TMP[i];
        } else {                               /* μ = -1 → denominator */
            mul_by_xminus1(D, degD, d, TMP);
            degD += d;
            for (int i = 0; i <= degD; ++i) D[i] = TMP[i];
        }
    }

    /* divide N by D to obtain Φ_1722(x) → Q[0..DEG_Q] */
    seq_t R[DEG_N + 1];
    for (int i = 0; i <= degN; ++i) R[i] = N[i];
    for (int i = 0; i <= DEG_Q; ++i) Q[i] = 0;

    for (int pos = degN; pos >= degD; --pos) {
        seq_t factor = R[pos];
        int qpos = pos - degD;
        Q[qpos] = factor;
        for (int j = 0; j <= degD; ++j) {
            R[qpos + j] -= factor * D[j];
        }
    }

    /* compute the sequence using the recurrence
     * a(0) = 1,
     * a(n) = - Σ_{j=1}^{min(n,DEG_Q)} Q[j] * a(n-j)   for n > 0  */
    seq_arr[0] = 1;
    for (int k = 1; k < PERIOD; ++k) {
        seq_t sum = 0;
        int limit = (k < DEG_Q) ? k : DEG_Q;
        for (int j = 1; j <= limit; ++j) {
            sum += Q[j] * seq_arr[k - j];
        }
        seq_arr[k] = -sum;
    }
}

/* public function */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_sequence();
        initialized = true;
    }
    return seq_arr[(size_t)n % PERIOD];
}
