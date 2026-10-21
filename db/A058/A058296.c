/*
 * A058296  Average of consecutive primes.
 * https://oeis.org/A058296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (prime(2n-2) + prime(2n-1)) / 2 (with prime(1)=2)
 * Range:     u8 n <= 27, u16 n <= 3271, u32 n <= 9245, u64 n <= 9245,
 *            u128 n <= 9245, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes and caches primes incrementally.  The cache is indexed
   0‑based so prime_array[0] = 2 (prime(1)).  It expands only when
   more primes are required.  The algorithm checks divisibility only
   by primes up to sqrt(cand), avoiding floating‑point operations. */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *prime_array = NULL;    /* dynamic array of primes */
static size_t prime_count = 0;          /* number of primes stored */
static size_t prime_capacity = 0;       /* allocated capacity */
static uint32_t next_candidate = 3;     /* next odd candidate to test */

/* Ensures that at least 'need' primes are available (1‑based index). */
static void ensure_primes(idx_t need)
{
    if (prime_array == NULL) {
        /* initialise with the first prime */
        prime_capacity = 16;
        prime_array = (uint32_t*)malloc(prime_capacity * sizeof(uint32_t));
        prime_array[0] = 2;
        prime_count = 1;
        /* next_candidate is already 3 */
    }

    while (prime_count < need) {
        uint32_t cand = next_candidate;
        int is_prime = 1;

        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_array[i];
            /* stop early if p*p > cand */
            if ((uint64_t)p * p > cand) {
                break;
            }
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            /* grow array if necessary */
            if (prime_count == prime_capacity) {
                size_t new_cap = prime_capacity == 0 ? 16 : prime_capacity * 2;
                uint32_t *tmp = (uint32_t*)realloc(prime_array, new_cap * sizeof(uint32_t));
                prime_array = tmp;
                prime_capacity = new_cap;
            }
            prime_array[prime_count++] = cand;
        }
        next_candidate += 2;   /* skip even numbers */
    }
}

/* Public sequence function, according to the required signature. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)2;
    }

    /* indices of the two consecutive primes (1‑based) */
    idx_t idx1 = 2 * n - 2;   /* prime(2n-2) */
    idx_t idx2 = 2 * n - 1;   /* prime(2n-1) */

    ensure_primes(idx2);      /* make sure we have both primes */

    seq_t p1 = (seq_t)prime_array[idx1 - 1];
    seq_t p2 = (seq_t)prime_array[idx2 - 1];

    return (p1 + p2) / 2;
}
