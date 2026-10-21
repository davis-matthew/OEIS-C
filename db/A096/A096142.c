/*
 * A096142  Let p(0) = 1, p(k) = k-th prime for k >= 1; write 2n = p(i) + p(j)
 *          with i <= j and i minimal; sequence gives i.
 * https://oeis.org/A096142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A096142: a(n) = the smallest i such that 2*n = p_i + p_j
   with i ≤ j, where p_0 = 1 and p_k (k≥1) is the k‑th prime. */

/* We compute primes up to the largest 2*n requested, extending the sieve
   on demand.  The algorithm is O(π(2n)) for each query, and the sieve
   construction is O(limit log log limit).  All arithmetic is done in
   seq_t, converting to idx_t only for array indexing.  The routine
   returns 0 when no representation is found, but for the numbers
   occurring in OEIS the representation always exists. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Static state for the on‑demand sieve */
static bool initialized = false;
static idx_t max_prime_limit = 0;           /* current sieve limit, inclusive */
static bool *is_prime = NULL;               /* is_prime[x] == true if x is prime (1 is considered prime) */
static int *prime_index = NULL;             /* prime_index[x] == index i of prime x, or -1 */
static seq_t *prime_list = NULL;            /* prime_list[i] == p_i, 0th entry = 1 */
static size_t prime_count = 0;              /* number of primes stored (including 1) */

/* ------------------------------------------------------------ */
/* Build a sieve up to "limit" (inclusive).  The caller guarantees
   that limit > max_prime_limit.  All arrays are reallocated.
   (Note: "seq_t" may be an arbitrary‑precision type, but the primes
   themselves are small enough to fit in a 64‑bit index.) */
static void build_sieve(idx_t limit)
{
    /* Allocate/resize arrays */
    free(is_prime);
    free(prime_index);
    free(prime_list);

    is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    prime_index = (int *)calloc((size_t)(limit + 1), sizeof(int));
    prime_list = (seq_t *)calloc((size_t)(limit + 1), sizeof(seq_t));

    if (!is_prime || !prime_index || !prime_list) {
        /* Allocation failure – abort */
        exit(1);
    }

    /* Initial values */
    for (idx_t i = 0; i <= limit; ++i) {
        is_prime[i] = true;
        prime_index[i] = -1;
    }
    is_prime[0] = false;
    /* Treat 1 as "prime" for this problem */
    is_prime[1] = true;

    /* Classic Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Build prime list and index map.  Store 1 as the first element. */
    prime_count = 0;
    prime_list[prime_count++] = (seq_t)1;    /* p_0 = 1 */
    prime_index[1] = 0;

    for (idx_t v = 2; v <= limit; ++v) {
        if (is_prime[v]) {
            prime_index[(int)v] = (int)prime_count;
            prime_list[prime_count++] = (seq_t)v;
        }
    }

    max_prime_limit = limit;
    initialized = true;
}

/* Ensure that the sieve covers at least "limit".  If necessary extend it. */
static void ensure_sieve(idx_t limit)
{
    if (limit <= max_prime_limit)
        return;

    /* Grow limit, at least doubling each time to keep rebuilds logarithmic. */
    idx_t new_limit = max_prime_limit ? max_prime_limit : 64;
    while (new_limit < limit) {
        new_limit <<= 1;   /* double */
    }
    /* Clamp in case of overflow (should not happen for test ranges). */
    if (new_limit < limit)
        new_limit = limit;
    build_sieve(new_limit);
}

/* ------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Target even number */
    seq_t target = (seq_t)n * ((seq_t)2);

    /* Ensure the sieve is large enough to test all primes up to 'target' */
    if ((idx_t)target > max_prime_limit)
        ensure_sieve((idx_t)target);

    /* Search for the smallest i with a valid representation */
    for (size_t i = 0; i < prime_count; ++i) {
        seq_t pi = prime_list[i];
        if (pi > target)
            break;            /* larger primes cannot contribute */

        seq_t needed = target - pi;
        /* 'needed' must be within the sieve range to index the arrays */
        if (needed > (seq_t)max_prime_limit)
            continue;        /* cannot look this up */

        idx_t req = (idx_t)needed;   /* safe cast: needed <= max_prime_limit */
        if (is_prime[req]) {
            int j = prime_index[(int)req];
            if (j >= 0 && (size_t)j >= i) {
                return (seq_t)i;
            }
        }
    }

    /* Theoretically unreachable for the domain of the sequence */
    return 0;
}
