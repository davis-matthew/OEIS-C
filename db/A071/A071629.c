/*
 * A071629  Number of terms in the set InvPhi(4*prime(n)), where prime(n) is
 *          the n-th prime.
 * https://oeis.org/A071629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS A071629 is: a(n) = |{ k : φ(k) = 4·prime(n) }|
   This implementation pre‑computes φ(k) for all k up to 8·PRIME_LIMIT
   (PRIME_LIMIT = 200 000, which exceeds the 10 000th prime).  For each
   k, if φ(k) is a multiple of 4, we insert the corresponding prime
   index into the answer array.  The function returns the stored count
   for the requested prime index.
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* externally supplied integer types */
extern seq_t a(idx_t n);

/* static state */
static bool initialized = false;
static seq_t *ans_arr = NULL;   /* 1‑based: ans_arr[i] = a(i) */
static idx_t  primes_cnt = 0;   /* number of primes */
static idx_t  max_prime   = 0;   /* largest prime needed */

/* ------------------------------------------------------------ */
/* Helper: linear sieve for primes up to LIMIT and totients up to
   8·LIMIT.  The answer array is filled during the totient loop.
*/
static void init_primes_totients(void)
{
    const idx_t PRIME_LIMIT = 200000;                         /* > 10 000th prime */
    const idx_t MAX_K      = 8 * PRIME_LIMIT;                 /* φ(k) ≤ k ≤ MAX_K */

    /* ---------- generate prime list and index mapping ---------- */
    bool *is_composite = (bool*)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_composite) exit(1);
    memset(is_composite, 0, (PRIME_LIMIT + 1));

    idx_t *primes_list = (idx_t*)malloc((PRIME_LIMIT + 1) * sizeof(idx_t));
    if (!primes_list) exit(1);
    idx_t *prime_idx   = (idx_t*)malloc((PRIME_LIMIT + 1) * sizeof(idx_t));
    if (!prime_idx) exit(1);
    memset(prime_idx, 0, (PRIME_LIMIT + 1) * sizeof(idx_t));

    primes_cnt = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            ++primes_cnt;
            primes_list[primes_cnt] = i;
            prime_idx[i] = primes_cnt;          /* 1‑based index */
        }
        for (idx_t j = 1; j <= primes_cnt; ++j) {
            idx_t p = primes_list[j];
            idx_t ip = i * p;
            if (ip > PRIME_LIMIT) break;
            is_composite[ip] = true;
            if (i % p == 0) break;
        }
    }
    max_prime = primes_list[primes_cnt];

    /* ---------- allocate answer array ---------- */
    ans_arr = (seq_t*)malloc((primes_cnt + 1) * sizeof(seq_t));
    if (!ans_arr) exit(1);
    memset(ans_arr, 0, (primes_cnt + 1) * sizeof(seq_t));

    /* ---------- linear sieve for φ up to MAX_K ---------- */
    uint32_t *phi = (uint32_t*)malloc((MAX_K + 1) * sizeof(uint32_t));
    if (!phi) exit(1);
    bool *is_prime_small = (bool*)calloc((MAX_K + 1), sizeof(bool));
    if (!is_prime_small) exit(1);
    uint32_t *primes_small = (uint32_t*)malloc((MAX_K + 1) * sizeof(uint32_t));
    if (!primes_small) exit(1);
    idx_t primes_small_cnt = 0;

    phi[0] = 0;
    phi[1] = 1;

    for (idx_t n = 2; n <= MAX_K; ++n) {
        if (!is_prime_small[n]) {
            is_prime_small[n] = true;        /* mark as prime */
            primes_small[primes_small_cnt++] = (uint32_t)n;
            phi[n] = (uint32_t)(n - 1);
        }
        for (idx_t j = 0; j < primes_small_cnt; ++j) {
            uint32_t p = primes_small[j];
            idx_t ip = n * p;
            if (ip > MAX_K) break;
            is_prime_small[ip] = true;          /* composite */
            if (n % p == 0) {
                phi[ip] = phi[n] * p;
                break;
            } else {
                phi[ip] = phi[n] * (p - 1);
            }
        }
    }

    /* ---------- populate answer counts ---------- */
    for (idx_t k = 1; k <= MAX_K; ++k) {
        uint32_t v = phi[k];
        if (v % 4 != 0) continue;
        idx_t p = v / 4;
        if (p <= max_prime && prime_idx[p] != 0) {
            ans_arr[prime_idx[p]] += (seq_t)1;
        }
    }

    /* ---------- free temporary arrays ---------- */
    free(is_composite);
    free(primes_list);
    free(prime_idx);
    free(phi);
    free(is_prime_small);
    free(primes_small);
}

/* ------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_primes_totients();
        initialized = true;
    }
    if (n == 0 || n > primes_cnt) return (seq_t)0;
    return ans_arr[n];
}
