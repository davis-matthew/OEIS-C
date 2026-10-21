/*
 * A216731  Primes p > 3 such that there is no power of 3 in the open interval
 *          (2p, 3p).
 * https://oeis.org/A216731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 2376, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: brute‑force generation of primes > 3 that contain no power of 3
   in the open interval (2p, 3p).  */
#include <stdlib.h>
#include <stdbool.h>

/* -------------------------------------------------------------- */
/*  Cached values of the sequence                                  */
static seq_t *a_cache = NULL;   /* array of seq_t  */
static size_t a_count = 0;      /* number of cached terms */
static size_t a_cap   = 0;      /* capacity of a_cache */

/* -------------------------------------------------------------- */
/*  Primes used for trial division                                 */
static uint64_t *prime_all = NULL;  /* array of uint64_t */
static size_t prime_count = 0;      /* number of stored primes */
static size_t prime_cap   = 0;      /* capacity of prime_all */

/* -------------------------------------------------------------- */
/*  Next odd candidate to test for primality                     */
static uint64_t next_candidate = 5;   /* start after 3 */

/* -------------------------------------------------------------- */
/* Ensure that an array of seq_t has at least `need`
   capacity (used for the sequence cache).                       */
static void
ensure_seq_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (need > *cap) {
        size_t newcap = (*cap == 0) ? 4 : (*cap << 1);
        while (newcap < need) newcap <<= 1;
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(**arr));
        *cap = newcap;
    }
}

/* Ensure that an array of uint64_t has at least `need` capacity. */
static void
ensure_primes_cap(size_t need)
{
    if (need > prime_cap) {
        size_t newcap = (prime_cap == 0) ? 4 : (prime_cap << 1);
        while (newcap < need) newcap <<= 1;
        prime_all = (uint64_t *)realloc(prime_all,
                                        newcap * sizeof(*prime_all));
        prime_cap = newcap;
    }
}

/* -------------------------------------------------------------- */
/* Initialise the small primes 2 and 3 used for trial division.   */
static void
init_small_primes(void)
{
    if (prime_count == 0) {
        ensure_primes_cap(2);
        prime_all[0] = 2;
        prime_all[1] = 3;
        prime_count = 2;
    }
}

/* -------------------------------------------------------------- */
/* Main sequence function                                          */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for 0 */

    /* Make sure we have the prime list ready for testing. */
    init_small_primes();

    /* Return cached value if already available. */
    if ((size_t)(n - 1) < a_count) return a_cache[n - 1];

    /* Grow the cache until it contains at least n terms. */
    while (a_count < (size_t)n) {
        uint64_t p = next_candidate;
        next_candidate += 2;                /* only odd numbers */

        /* Trial division using primes already found. */
        bool prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t q = prime_all[i];
            if (q * q > p) break;           /* sqrt(p) reached */
            if (p % q == 0) {
                prime = false;
                break;
            }
        }
        if (!prime) continue;               /* composite, skip */

        /* Store the new prime for future divisions. */
        ensure_primes_cap(prime_count + 1);
        prime_all[prime_count++] = p;

        /* Skip p <= 3, we only want primes > 3. */
        if (p <= 3) continue;

        /* Check the property:
           no power of 3 lies strictly between 2p and 3p. */
        seq_t tp  = (seq_t)p;
        seq_t t2  = tp * (seq_t)2;
        seq_t t3  = tp * (seq_t)3;
        seq_t power = (seq_t)3;

        while (power <= t2) power *= (seq_t)3;   /* find first 3^k > 2p */

        if (power < t3)   /* a power of 3 falls inside (2p,3p) */
            continue;     /* reject this prime */

        /* Accept the prime and cache it. */
        ensure_seq_cap(&a_cache, &a_cap, a_count + 1);
        a_cache[a_count++] = tp;
    }

    return a_cache[n - 1];
}
