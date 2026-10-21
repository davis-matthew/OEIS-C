/*
 * A077665  Final term of n-th row of A077664.
 * https://oeis.org/A077665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + the n-th coprime integer of n * (i.e., a(n) = n +
 *            A069213(n)), equivalently a(n) = n + the * (n+φ(n))‑th integer
 *            coprime to n. For n ≥ 1. The function below * computes φ(n) by
 *            trial division and then finds the required * residue by a simple
 *            scan, all in O(√n + φ(n) + n) time per term
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses only the basic C arithmetic operators and
 *  works with the arbitrary‑precision `seq_t` and 64‑bit `idx_t`
 *  supplied by the build system. */

#include <stdint.h>

/* Euclidean algorithm for gcd on idx_t values. */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != (idx_t)0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Euler's totient function φ(n) computed by trial division. */
static seq_t phi_local(idx_t n)
{
    seq_t result = (seq_t)n;
    idx_t tmp = n;
    for (idx_t p = (idx_t)2; p * p <= tmp; ++p) {
        if (tmp % p == (idx_t)0) {
            while (tmp % p == (idx_t)0)
                tmp /= p;
            result = result / (seq_t)p * ((seq_t)p - (seq_t)1);
        }
    }
    if (tmp > (idx_t)1)
        result = result / (seq_t)tmp * ((seq_t)tmp - (seq_t)1);
    return result;
}

seq_t a(idx_t n)
{
    /* φ(n) fits in idx_t because φ(n) ≤ n for all n. */
    seq_t phi_seq = phi_local(n);
    idx_t phi_val = (idx_t)phi_seq;

    /* Determine the block and position of the n‑th coprime residue. */
    idx_t block = (n - (idx_t)1) / phi_val;
    idx_t pos   = (n - (idx_t)1) % phi_val;

    /* Scan for the residue r that is the (pos+1)‑th coprime modulo n. */
    idx_t r = (idx_t)0;
    idx_t count = (idx_t)0;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        if (gcd_idx(i, n) == (idx_t)1) {
            if (count == pos) {
                r = i;
                break;
            }
            ++count;
        }
    }

    /* The n‑th coprime integer equals block * n + r. */
    seq_t nth_coprime = (seq_t)block * (seq_t)n + (seq_t)r;

    /* a(n) = n + nth_coprime. */
    return (seq_t)n + nth_coprime;
}
