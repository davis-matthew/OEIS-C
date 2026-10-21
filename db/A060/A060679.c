/*
 * A060679  Orders of non-cyclic groups.
 * https://oeis.org/A060679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Orders of non‑cyclic groups. 
   An integer n is included if a non‑cyclic group of order n exists,
   i.e. iff n has a repeated prime factor or there exist distinct
   primes p,q|n with p|(q-1). */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/*--- Prime sieve -----------------------------------------------*/
#define PRIME_LIMIT 200000       /* sufficient for all needed factors */
static unsigned char is_prime_flag[PRIME_LIMIT + 1];
static uint32_t prime_list[17000];
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready)
        return;

    for (size_t i = 0; i <= PRIME_LIMIT; ++i)
        is_prime_flag[i] = 1;
    is_prime_flag[0] = is_prime_flag[1] = 0;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (is_prime_flag[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime_flag[j] = 0;

    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime_flag[i])
            prime_list[prime_cnt++] = (uint32_t)i;

    primes_ready = true;
}

/*--- Test for existence of a non‑cyclic group of order n --------*/
static bool has_non_cyclic_group(uint64_t n)
{
    if (n < 4)            /* groups of order <4 are cyclic or undefined */
        return false;

    uint64_t m = n;
    int primes[10];     /* prime factors, at most 10 for n ≤ 200 000 */
    int exps[10];
    size_t cnt = 0;
    bool repeated = false;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > m)
            break;
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (e > 1)
                repeated = true;
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {            /* remaining prime factor > sqrt(original) */
        primes[cnt] = (int)m;
        exps[cnt] = 1;
        ++cnt;
    }

    if (repeated)               /* repeated prime factor → non‑cyclic */
        return true;

    if (cnt < 2)                /* prime order → cyclic */
        return false;

    /* squarefree case: check p | (q-1) for some distinct primes p,q */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            uint64_t p = primes[i], q = primes[j];
            if (((q - 1) % p == 0) || ((p - 1) % q == 0))
                return true;
        }
    }
    return false;       /* squarefree and no such pair → cyclic */
}

/*--- Dynamic array for pre‑computed sequence ------------------*/
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t next_candidate = 4;   /* next integer to test */

/* Ensure capacity for at least 'need' elements */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    if (seq_cap == 0)
        seq_cap = 16;
    while (seq_cap < need)
        seq_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
}

/*--- Sequence function ----------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined, but safe */

    init_primes();

    /* if we already have at least n terms compute directly */
    if (seq_len < (size_t)n) {
        /* generate until we have n terms */
        while (seq_len < (size_t)n) {
            if (has_non_cyclic_group(next_candidate)) {
                ensure_seq_capacity(seq_len + 1);
                seq_arr[seq_len++] = (seq_t)next_candidate;
            }
            ++next_candidate;
        }
    }
    return seq_arr[n - 1];
}
