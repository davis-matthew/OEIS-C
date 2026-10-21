/*
 * A225936  Number of conjugacy classes in simply connected Chevalley group
 *          E_7(q) as q runs through the prime powers.
 * https://oeis.org/A225936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Let q be the n-th prime power. Then, a(n) = q^7 + q^6 +
 *            2q^5 + 4q^4 + 10q^3 + 15q^2 + 25q + 21 if q ≡ 0 (mod 2) a(n) =
 *            q^7 + q^6 + 2q^5 + 7q^4 + 17q^3 + 35q^2 + 70q + 99 if q ≡ 0 (mod
 *            3) a(n) = q^7 + q^6 + 2q^5 + 7q^4 + 17q^3 + 35q^2 + 71q + 103
 *            otherwise
 * Range:     u8 none, u16 none, u32 n <= 13, u64 n <= 124, u128 n <= 1000,
 *            big any n
 * Helpers:   alloc_seq_array, build_primes_upto, compare_seq_t,
 *            free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/compare_seq_t.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Global storage for pre‑computed prime powers */
static seq_t *prime_powers = NULL;
static size_t prime_powers_cnt = 0;
static bool prime_powers_init = false;

/* Append a new element to a dynamic seq_t array */
static void append_seq_t(seq_t **arr, size_t *cnt, size_t *cap, seq_t val)
{
    if (*cnt == *cap) {
        size_t new_cap = (*cap == 0) ? 32 : (*cap << 1);
        seq_t *tmp = alloc_seq_array(new_cap);
        for (size_t i = 0; i < *cnt; ++i) tmp[i] = (*arr)[i];
        free_seq_array(*arr);
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*cnt] = val;
    ++(*cnt);
}

/* Pre‑compute all prime powers up to an upper bound */
static void init_prime_powers(void)
{
    if (prime_powers_init) return;

    /* Upper bound large enough for n up to ~10 000 */
    const idx_t LIMIT = 300000;

    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(LIMIT, &primes, &primes_cnt);

    seq_t *arr = NULL;
    size_t cap = 0, cnt = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t val = (seq_t)p;
        while (1) {
            append_seq_t(&arr, &cnt, &cap, val);

            seq_t next = val * (seq_t)p;
            if (next > (seq_t)LIMIT) break;
            if (next < val) break;       /* safety against overflow */
            val = next;
        }
    }
    free(primes);

    qsort(arr, cnt, sizeof(seq_t), compare_seq_t);

    prime_powers      = arr;
    prime_powers_cnt  = cnt;
    prime_powers_init = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n = 0 */

    init_prime_powers();

    /* For safety, if the pre‑computed bound is insufficient,
       we could extend it. That is unlikely for n up to 10 000 */
    if ((size_t)(n - 1) >= prime_powers_cnt) {
        /* Not expected. Return 0 to avoid undefined behaviour. */
        return (seq_t)0;
    }

    seq_t q  = prime_powers[n - 1];
    seq_t q2 = q * q;
    seq_t q3 = q2 * q;
    seq_t q4 = q3 * q;
    seq_t q5 = q4 * q;
    seq_t q6 = q5 * q;
    seq_t q7 = q6 * q;

    seq_t res;
    if ((q % (seq_t)2) == 0) {
        res = q7 + q6 + 2 * q5 + 4 * q4 + 10 * q3 + 15 * q2 + 25 * q + 21;
    } else if ((q % (seq_t)3) == 0) {
        res = q7 + q6 + 2 * q5 + 7 * q4 + 17 * q3 + 35 * q2 + 70 * q + 99;
    } else {
        res = q7 + q6 + 2 * q5 + 7 * q4 + 17 * q3 + 35 * q2 + 71 * q + 103;
    }
    return res;
}
