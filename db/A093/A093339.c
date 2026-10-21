/*
 * A093339  Middle digits of primes with an odd number of digits.
 * https://oeis.org/A093339
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A093339 – Middle digits of primes with an odd number of digits.
 *
 *  a(n) = middle decimal digit of the (n–2)-th prime that has an odd
 *  number of decimal digits.  The sequence is offset 2: a(2)=2
 *  (the first odd‑digit prime 2), a(3)=3, a(4)=5, …
 *
 *  The implementation builds primes up to a growing limit using a
 *  simple sieve of Eratosthenes.  Whenever the stored list of odd‑digit
 *  primes is too short for a requested index, the limit is doubled
 *  and the sieve is run again.  This is fast enough for n≈10 000.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/*  type definitions provided by the build system */
extern seq_t a(idx_t n);
static size_t prime_limit   = 0;      /* current sieve upper bound          */
static size_t odd_count     = 0;      /* number of odd‑digit primes stored  */
static seq_t *odd_primes    = NULL;   /* dynamic array of odd‑digit primes  */
static size_t odd_cap      = 0;       /* capacity of odd_primes             */

/* ------------------------------------------------------------------ */
/* Compute the number of decimal digits of a positive integer.        */
static int num_digits_uint64(uint64_t x)
{
    int d = 0;
    while (x) { d++; x /= 10; }
    return d;
}

/* ------------------------------------------------------------------ */
/* Rebuild the odd‑digit prime list up to new_limit.                   */
static void rebuild_up_to(size_t new_limit)
{
    bool *is_prime = (bool*)malloc((new_limit + 1) * sizeof(bool));
    if (!is_prime) exit(1);

    /* Initialise sieve */
    for (size_t i = 0; i <= new_limit; ++i) is_prime[i] = true;
    if (new_limit >= 0) is_prime[0] = false;
    if (new_limit >= 1) is_prime[1] = false;

    /* Eratosthenes */
    size_t sq = (size_t)(sqrt((double)new_limit) + 0.5);
    for (size_t i = 2; i <= sq; ++i) {
        if (!is_prime[i]) continue;
        for (size_t j = i * i; j <= new_limit; j += i) {
            is_prime[j] = false;
        }
    }

    /* Reallocate storage for odd‑digit primes */
    free(odd_primes);
    odd_primes = NULL;
    odd_cap    = 0;
    odd_count  = 0;

    /* Collect odd‑digit primes in ascending order */
    for (size_t i = 2; i <= new_limit; ++i) {
        if (!is_prime[i]) continue;
        uint64_t p = (uint64_t)i;
        int d = num_digits_uint64(p);
        if (d % 2 == 0) continue;           /* even number of digits → skip */

        if (odd_count >= odd_cap) {
            odd_cap = odd_cap ? odd_cap * 2 : 64;
            odd_primes = (seq_t*)realloc(odd_primes, odd_cap * sizeof(seq_t));
            if (!odd_primes) exit(1);
        }
        odd_primes[odd_count++] = (seq_t)p;
    }

    free(is_prime);
    prime_limit = new_limit;
}

/* ------------------------------------------------------------------ */
/* Ensure that at least (need+1) odd‑digit primes are stored.        */
static void ensure_odd_primes(idx_t need)
{
    if (!odd_primes) {
        /* Initial limit – adjust as needed */
        rebuild_up_to(200000);
    }
    while ((size_t)odd_count <= need) {
        size_t new_limit = prime_limit * 2;
        rebuild_up_to(new_limit);
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;    /* values below the offset are undefined */

    size_t need = (size_t)(n - 2);   /* zero‑based index into odd_primes */

    ensure_odd_primes((idx_t)need);

    seq_t prime = odd_primes[need];
    int d = num_digits_uint64((uint64_t)prime);
    int exp = (d - 1) / 2;

    uint64_t divisor = 1;
    for (int i = 0; i < exp; ++i) divisor *= 10ULL;

    seq_t div_seq = (seq_t)divisor;
    seq_t middle   = (prime / div_seq) % (seq_t)10;

    return middle;
}
