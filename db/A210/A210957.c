/*
 * A210957  Prime pair (p, q), p<=q, such that p + q = 2*n and p*q is the
 *          minimal product.
 * https://oeis.org/A210957
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = interleaved sequence of the minimal‐product Goldbach *
 *            partitions of 2n. For each n ≥ 2, * let k = n/2 + 1 (integer
 *            division). * If n is even, a(n) = smallest prime p (call p_k)
 *            such that * p + q = 2k with q prime and p*q minimal, i.e. the
 *            first prime p * ≤ k with (2k – p) prime. * If n is...
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static idx_t max_k_computed = 1;          /* greatest k for which data exist  */
static seq_t *P_arr = NULL;              /* P_arr[k] = p_k  */
static seq_t *Q_arr = NULL;              /* Q_arr[k] = q_k  */

static idx_t *prime_list = NULL;         /* list of primes up to current limit */
static size_t prime_cnt = 0;

/* Build prime table and the p_k, q_k arrays up to kmax (inclusive)   */
static void ensure_max_k(idx_t kmax)
{
    if (kmax <= max_k_computed) return;

    /* 1. Build a prime sieve up to limit = 2*kmax + 10            */
    const idx_t limit = 2 * kmax + 10;
    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return;  /* out of memory – in practice this won't happen */

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* 2. Collect primes into a simple array                      */
    free(prime_list);
    prime_list = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    if (!prime_list) { free(is_prime); return; }
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_list[prime_cnt++] = i;

    /* 3. Allocate/reallocate P_arr and Q_arr up to kmax            */
    seq_t *P_new = (seq_t*)malloc((kmax + 1) * sizeof(seq_t));
    seq_t *Q_new = (seq_t*)malloc((kmax + 1) * sizeof(seq_t));
    if (!P_new || !Q_new) { /* out of memory */
        free(is_prime); free(prime_list);
        free(P_arr); free(Q_arr);
        return;
    }

    /* copy previous data if any */
    if (P_arr) {
        memcpy(P_new, P_arr, (max_k_computed + 1) * sizeof(seq_t));
        memcpy(Q_new, Q_arr, (max_k_computed + 1) * sizeof(seq_t));
        free(P_arr); free(Q_arr);
    } else {
        P_new[0] = 0; P_new[1] = 0;
        Q_new[0] = 0; Q_new[1] = 0;
    }

    /* 4. Compute p_k, q_k for k = max_k_computed+1 .. kmax          */
    for (idx_t k = max_k_computed + 1; k <= kmax; ++k) {
        for (size_t pi = 0; pi < prime_cnt && prime_list[pi] <= k; ++pi) {
            idx_t p = prime_list[pi];
            idx_t q = 2 * k - p;
            if (q <= limit && is_prime[q]) {
                P_new[k] = (seq_t)p;
                Q_new[k] = (seq_t)q;
                break;
            }
        }
    }

    /* 5. Replace old arrays with new ones                        */
    free(is_prime);
    P_arr = P_new;
    Q_arr = Q_new;
    max_k_computed = kmax;
}

/* Public function required by the test harness                  */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 2.  For safety we ignore
     * smaller indices (they are unused by the tests).               */
    if (n < 2) return (seq_t)0;

    /* Compute the needed k = n/2 + 1                                     */
    idx_t k = n / 2 + 1;
    ensure_max_k(k);

    /* Even n -> return p_k, odd n -> return q_k                         */
    if ((n & 1) == 0) return P_arr[k];
    else return Q_arr[k];
}
