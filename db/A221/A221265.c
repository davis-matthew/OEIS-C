/*
 * A221265  Numbers having more distinct prime factors of form 4*k+1 than of
 *          4*k+3.
 * https://oeis.org/A221265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A221265 – numbers having more distinct prime factors ≡1 mod 4 than ≡3 mod 4.  
   Computed by a linear search with a prime sieve. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/*----- dynamic array for the result sequence --------------------------------*/
static void grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0 ? 8 : *cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/*----- prime sieve ----------------------------------------------------------*/
static uint32_t *primes = NULL;
static size_t primes_count = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;
    /* limit chosen to cover sqrt of numbers up to about 10^12,
       which is far beyond the 10 000‑th term of this sequence */
    const uint32_t LIMIT = 200000;
    uint8_t *is_prime = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
    if (!is_prime) return;                  /* out of memory */

    for (uint32_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i) is_prime[j] = 0;
        }
    }

    /* count primes to allocate array */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++cnt;

    primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    primes_count = cnt;
    size_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) primes[idx++] = i;

    free(is_prime);
}

/*----- test whether a number satisfies the condition -----------------------*/
static bool passes_condition(uint64_t x)
{
    if (x < 5) return false;                /* first term is 5 */
    uint64_t tmp = x;
    int cnt1 = 0, cnt3 = 0;                 /* distinct 1 mod 4, 3 mod 4 */

    for (size_t i = 0; i < primes_count &&
         (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            if (p % 4 == 1) ++cnt1;
            else if (p % 4 == 3) ++cnt3;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {                          /* remaining prime factor */
        if (tmp % 4 == 1) ++cnt1;
        else if (tmp % 4 == 3) ++cnt3;
    }
    return (cnt1 > cnt3);
}

/*----- storage for the sequence --------------------------------------------*/
static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static uint64_t current = 1;                /* last checked integer */

/*----- public function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                      /* undefined for offset 0 */
    init_primes();

    while (seq_len < (size_t)n) {
        ++current;
        if (passes_condition(current)) {
            grow_seq(&seq, &seq_cap, seq_len + 1);
            seq[seq_len++] = (seq_t)current;           /* cast to seq_t */
        }
    }
    return seq[n - 1];
}
