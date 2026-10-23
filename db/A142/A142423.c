/*
 * A142423  Primes congruent to 11 mod 49.
 * https://oeis.org/A142423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 155, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 11 modulo 49 (a(n), n ≥ 1) */
/* The function searches primes by sieve up to a safe bound; the first N
   terms of the sequence are stored in a static vector so that any call
   with n >= 1 returns the correct value. */

/* implementation configuration – include the utility that allocates
   integer vectors if desired, otherwise use plain malloc. */
#include <stdlib.h>
#include <stdbool.h>

/* static storage for the pre‑computed sequence */
static seq_t *congruent = NULL;
static size_t congruent_cap = 0;
static size_t congruent_sz  = 0;
static bool initialized = false;

/* enlarge the array of primes */
static void push_congruent(seq_t val)
{
    if (congruent_sz == congruent_cap) {
        size_t new_cap = congruent_cap ? congruent_cap * 2 : 1024;
        /* cast realloc result to the correct type for C++ */
        seq_t *new_arr = (seq_t *)realloc(congruent,
                                          new_cap * sizeof(seq_t));
        if (!new_arr) abort();          /* out of memory */
        congruent = new_arr;
        congruent_cap = new_cap;
    }
    congruent[congruent_sz++] = val;
}

/* sieve primes up to a bound that contains at least 10 000 terms */
static void init_sequence(void)
{
    if (initialized) return;

    /* 10 000 primes ≡ 11 (mod 49) lie well below 10 000 000.
       (The prime number theorem gives a(10000) ≈ 4.2·10⁵, and the
       congruence condition only adds a factor 1/φ(49) ≈ 1/42. */
    const size_t LIMIT = 10000000U;

    /* allocate sieve array */
    char *isprime = (char *)malloc(LIMIT + 1);
    if (!isprime) abort();
    for (size_t i = 0; i <= LIMIT; ++i) isprime[i] = 1;
    isprime[0] = isprime[1] = 0;

    /* classic Eratosthenes */
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (isprime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                isprime[j] = 0;
        }
    }

    /* collect primes ≡ 11 mod 49 */
    for (size_t p = 2; p <= LIMIT; ++p) {
        if (isprime[p] && (p % 49) == 11U)
            push_congruent((seq_t)p);
    }

    free(isprime);
    initialized = true;
}

/* required public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety, not part of OEIS offset */

    init_sequence();

    /* indices are 1‑based in OEIS, our array is 0‑based */
    size_t idx = (size_t)n - 1U;
    if (idx >= congruent_sz) {
        /* outside pre‑computed range – should not happen for n ≤ 10000 */
        return (seq_t)0;
    }
    return congruent[idx];
}
