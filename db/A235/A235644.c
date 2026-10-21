/*
 * A235644  Number of decompositions of 12*n into the sum of two (not
 *          necessarily distinct) twin prime pairs.
 * https://oeis.org/A235644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A235644 – Number of decompositions of 12*n into the sum of two
    twin prime pairs.

    For a given n the function counts unordered pairs of twin‑prime
    starts (p1,p1+2) and (p2,p2+2) satisfying

        p1 + p2 = 6*n - 2.
*/
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*  Static sieve and twin prime cache  */
static bool *prime_sieve   = NULL;   /* prime_sieve[x] is true if x is prime   */
static idx_t   sieve_limit = 0;     /* maximum number represented in sieve    */
static uint64_t *twin_start = NULL; /* start of each twin prime pair          */
static size_t  twin_cnt    = 0;     /* number of twin primes stored           */

/* Rebuilds sieve and twin list up to (inclusive) new_limit+2 */
static void
ensure_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit)
        return;                       /* already sufficient */

    /* Allocate new sieve up to new_limit + 2 */
    idx_t sz = new_limit + 3;           /* +3 to include index new_limit+2 */
    bool *sieve = (bool *)malloc(sz * sizeof(bool));
    if (!sieve) abort();               /* out of memory */

    for (idx_t i = 0; i < sz; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;

    /* Eratosthenes */
    for (idx_t i = 2; i * i <= new_limit + 2; ++i)
        if (sieve[i])
            for (idx_t j = i * i; j <= new_limit + 2; j += i)
                sieve[j] = false;

    /* Build twin prime starts list */
    size_t cap = (size_t)(new_limit / 2) + 16;   /* rough upper bound */
    uint64_t *twins = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!twins) abort();                       /* out of memory */

    size_t count = 0;
    for (idx_t p = 2; p + 2 <= new_limit + 2; ++p) {
        if (sieve[p] && sieve[p + 2]) {
            if (count >= cap) {               /* shouldn't happen, grow */
                cap <<= 1;
                twins = (uint64_t *)realloc(twins, cap * sizeof(uint64_t));
                if (!twins) abort();
            }
            twins[count++] = p;
        }
    }

    /* Replace old data */
    free(prime_sieve);
    free(twin_start);
    prime_sieve = sieve;
    twin_start  = twins;
    twin_cnt    = count;
    sieve_limit = new_limit + 2;              /* up to this index valid */
}

/*  Sequence function  */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n<1, return 0 */

    /* target = 6*n - 2  (twin starts sum) */
    seq_t target_seq = (seq_t)6 * n - (seq_t)2;
    idx_t target     = (idx_t)target_seq;      /* fits in idx_t for our limits */

    /* We need primality up to target + 2 (for p2+2) */
    ensure_sieve(target + 2);

    seq_t count = 0;
    idx_t half = target / 2;

    for (size_t i = 0; i < twin_cnt; ++i) {
        idx_t p1 = twin_start[i];
        if (p1 > half)
            break;                       /* prevent double counting */

        idx_t p2 = target - p1;
        if (p2 >= 3 &&
            p2 + 2 <= sieve_limit &&
            prime_sieve[p2] &&
            prime_sieve[p2 + 2]) {
            ++count;
        }
    }
    return count;
}
