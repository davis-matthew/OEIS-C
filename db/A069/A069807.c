/*
 * A069807  Smallest k>n such that prime(n)+1 divides prime(k)+1.
 * https://oeis.org/A069807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A069807: Smallest k>n such that prime(n)+1 divides prime(k)+1 */
/* (prime indices are 1‑based; a(1)=3, a(2)=4, etc.) */

/* The function builds a list of primes on demand.  It keeps a
   static dynamic array of the primes found so far (uint32_t is
   sufficient for n up to ~10 000).  For larger builds the array type
   can be widened if desired.  The search for k>n is performed by
   generating primes sequentially until the divisibility condition
   holds. */

#include <stdint.h>
#include <stdlib.h>

/* static data used by the sequence calculation */
static uint32_t *prime_array = NULL;      /* array of primes (0‑based)   */
static size_t    prime_cnt   = 0;         /* number of primes stored     */
static size_t    prime_cap   = 0;         /* current allocation capacity */
static uint32_t  next_trial  = 3;         /* next odd candidate to test  */

/* Append a prime to the dynamic array, expanding storage if needed */
static void add_prime(uint32_t p)
{
    if (prime_cap == 0) {
        prime_cap = 4;
        prime_array = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    } else if (prime_cnt >= prime_cap) {
        prime_cap <<= 1;
        prime_array = (uint32_t*)realloc(prime_array, prime_cap * sizeof(uint32_t));
    }
    prime_array[prime_cnt++] = p;
}

/* Ensure we have at least 'needed' primes stored (needed >= 1).  */
static void ensure_prime_count(size_t needed)
{
    if (prime_cnt >= needed)
        return;

    if (prime_cnt == 0) {     /* initialise with the first prime */
        add_prime(2);
        next_trial = 3;
    }

    while (prime_cnt < needed) {
        uint32_t cand = next_trial;
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt && (uint64_t)prime_array[i] * prime_array[i] <= cand; ++i) {
            if (cand % prime_array[i] == 0) { is_prime = 0; break; }
        }
        if (is_prime)
            add_prime(cand);
        next_trial += 2;  /* test only odd numbers after 2 */
    }
}

/* The required OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                     /* out of range but safe return */
        return (seq_t)0;

    /* Ensure we have the n-th prime (indices are 1‑based). */
    ensure_prime_count((size_t)n);

    uint64_t p_n   = (uint64_t)prime_array[n - 1];
    uint64_t divisor = p_n + 1ULL;      /* should divide p_k + 1 */

    /* Search for the smallest k > n satisfying the condition. */
    idx_t k = n + 1;
    for (;;) {
        ensure_prime_count((size_t)k);
        uint64_t p_k = (uint64_t)prime_array[k - 1];
        if ( (p_k + 1ULL) % divisor == 0ULL )
            return (seq_t)k;
        ++k;
    }
}
