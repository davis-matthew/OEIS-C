/*
 * A092126  Numbers n such that n, n+2, n+4, n+6 are semiprimes.
 * https://oeis.org/A092126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 431, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A092126: Numbers n such that n, n+2, n+4, n+6 are semiprimes.
    The implementation searches for such n by sieving semiprimes up to a
    dynamically expanding limit.  The sequence is cached so that calls
    with arbitrary indices are answered in O(1) after the necessary
    values have been generated.  The algorithm is O(N log log N) for
    generating the first N terms. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/build_primes_upto.h"   /* build_primes_upto(limit, &primes, &cnt) */
#include "../utils/seq_alloc.h"           /* seq_alloc(cap) -> seq_t*          */
#include "../utils/seq_realloc.h"        /* seq_realloc(old, oldcap, newcap) */

static inline bool get_bit(const uint8_t *arr, size_t pos)
{
    return (arr[pos >> 3] >> (pos & 7)) & 1U;
}
static inline void set_bit(uint8_t *arr, size_t pos)
{
    arr[pos >> 3] |= 1U << (pos & 7);
}

/*  Cache of sequence values ------------------------------------------ */
static seq_t *seq_cache = NULL;   /* array of first terms */
static size_t seq_cap   = 0;      /* allocated capacity      */
static size_t seq_len   = 0;      /* number of values cached */

/*  Bitset indicating which integers up to sieve_limit are semiprimes ---- */
static uint8_t *semiprime_bits = NULL;
static idx_t sieve_limit = 0;

/*  List of primes up to sieve_limit ----------------------------------- */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;

/*  Current odd candidate to test -------------------------------------- */
static idx_t next_candidate = 3;            /* first odd positive integer */

/*  Ensure that primes and semiprime bitset are available up to 'limit'. */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;        /* already big enough */

    /* Release old data */
    free(semiprime_bits);
    free(prime_list);
    semiprime_bits = NULL;
    prime_list = NULL;
    sieve_limit = 0;
    prime_cnt = 0;

    /* --- generate primes up to limit ----------------------------------- */
    {
        size_t cnt = 0;
        uint64_t *tmp = NULL;
        build_primes_upto(limit, &tmp, &cnt);
        prime_list = tmp;
        prime_cnt  = cnt;
    }

    /* --- allocate bitset for semiprimes -------------------------------- */
    size_t bytes = (size_t)limit >> 3U;           /* bits per byte = 8   */
    semiprime_bits = (uint8_t *)calloc(bytes + 1, 1);
    if (!semiprime_bits) return;                 /* out of memory, unlikely */

    /* --- mark all p*q with p <= q, p*q <= limit --------------------- */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > limit / p) break;                /* p*p > limit */
        for (size_t j = i; j < prime_cnt; ++j) {
            uint64_t q = prime_list[j];
            if (p > limit / q) break;            /* p*q > limit, avoid overflow */
            uint64_t prod = p * q;
            set_bit(semiprime_bits, (size_t)prod);
        }
    }
    sieve_limit = limit;
}

/*  Generate cache entries until we have at least 'desired' terms. */
static void generate_up_to(size_t desired)
{
    if (!seq_cache) {                            /* first allocation */
        seq_cap   = 1024;
        seq_cache = (seq_t *)seq_alloc(seq_cap);
    }
    if (seq_len >= desired) return;

    if (sieve_limit == 0) ensure_sieve(100000);  /* initial minimal sieve */

    while (seq_len < desired) {
        /* Extend sieve if necessary */
        if (next_candidate + 6 > sieve_limit) {
            idx_t new_limit = sieve_limit * 2 + 100000;   /* growth factor */
            ensure_sieve(new_limit);
            continue;
        }

        /* Test four consecutive odd numbers for semiprimality */
        if (get_bit(semiprime_bits, (size_t)next_candidate) &&
            get_bit(semiprime_bits, (size_t)(next_candidate + 2)) &&
            get_bit(semiprime_bits, (size_t)(next_candidate + 4)) &&
            get_bit(semiprime_bits, (size_t)(next_candidate + 6))) {

            /* Resize cache if needed */
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_cache = (seq_t *)seq_realloc(seq_cache, seq_cap / 2, seq_cap);
            }
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        next_candidate += 2;    /* skip even numbers */
    }
}

/*  Public function required by the framework -------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset is 1, behaviour for n=0 ignored */
    generate_up_to((size_t)n);
    return seq_cache[n - 1];
}
