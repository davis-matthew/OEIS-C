/*
 * A067747  Primes interleaved between composite numbers: n-th prime followed
 *          by the n-th composite number.
 * https://oeis.org/A067747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000040(n/2) if n is odd, A002808(n/2) if n is even.
 *            Where A000040 is the n-th prime, and A002808 is the n-th
 *            composite number
 * Range:     u8 n <= 108, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation keeps static arrays of primes and composites that
    are extended on-demand.  The sieve for primes is replaced with a
    simple trial-divison routine because the required limits (≈10000
    elements) are small enough for that approach.  The code is pure C,
    so it compiles as C11 and as C++17.  It uses only stdlib.h for
    dynamic allocation. */

#include <stdlib.h>   /* malloc, realloc, NULL, size_t */
#include <stdint.h>   /* uint64_t, size_t */

/* Forward declarations of the helper functions. */
static int is_prime_simple(uint64_t n);
static void grow_primes(void);
static void grow_composites(void);
static void push_prime(seq_t val);
static void push_composite(seq_t val);
static seq_t get_prime(idx_t k);
static seq_t get_composite(idx_t k);

/* Static storage for primes and composites. */
static seq_t *primes = NULL;       /* array of primes */
static size_t primes_len = 0;      /* number of primes stored */
static size_t primes_cap = 0;      /* current capacity of the array */

static seq_t *composites = NULL;   /* array of composites */
static size_t composites_len = 0;  /* number of composites stored */
static size_t composites_cap = 0;  /* current capacity of the array */

static uint64_t next_prime_candidate = 2;
static uint64_t next_composite_candidate = 4;

/* Test if a 64‑bit integer is prime.  The routine is sufficient for
   the values needed in this sequence (up to about 10^6). */
static int is_prime_simple(uint64_t n)
{
    if (n < 2) return 0;
    if (n == 2) return 1;
    if ((n & 1) == 0) return 0;
    for (uint64_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return 0;
    return 1;
}

/* Ensure the primes array can hold an additional element. */
static void grow_primes(void)
{
    size_t newcap = primes_cap ? primes_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(primes, newcap * sizeof(seq_t));
    if (!tmp) { /* allocation failed – aborting */
        abort();
    }
    primes = tmp;
    primes_cap = newcap;
}

/* Ensure the composites array can hold an additional element. */
static void grow_composites(void)
{
    size_t newcap = composites_cap ? composites_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(composites, newcap * sizeof(seq_t));
    if (!tmp) abort();
    composites = tmp;
    composites_cap = newcap;
}

/* Append a prime to the array. */
static void push_prime(seq_t val)
{
    if (primes_len == primes_cap) grow_primes();
    primes[primes_len++] = val;
}

/* Append a composite to the array. */
static void push_composite(seq_t val)
{
    if (composites_len == composites_cap) grow_composites();
    composites[composites_len++] = val;
}

/* Return the k-th prime (k >= 1). */
static seq_t get_prime(idx_t k)
{
    while (primes_len < k) {
        /* Advance candidate until a prime is found. */
        while (!is_prime_simple(next_prime_candidate))
            ++next_prime_candidate;
        push_prime((seq_t)next_prime_candidate);
        ++next_prime_candidate;
    }
    return primes[k - 1];
}

/* Return the k-th composite (k >= 1). */
static seq_t get_composite(idx_t k)
{
    while (composites_len < k) {
        /* Advance candidate until a composite >=4 is found. */
        while (next_composite_candidate <= 3 ||
               is_prime_simple(next_composite_candidate))
            ++next_composite_candidate;
        push_composite((seq_t)next_composite_candidate);
        ++next_composite_candidate;
    }
    return composites[k - 1];
}

/* Main function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but keep harmless */
    if (n & 1) {          /* odd index → prime */
        idx_t kth = (n + 1) / 2;
        return get_prime(kth);
    } else {              /* even index → composite */
        idx_t kth = n / 2;
        return get_composite(kth);
    }
}
