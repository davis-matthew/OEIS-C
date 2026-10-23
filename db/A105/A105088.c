/*
 * A105088  Sum of the sides of ordered 2 X 2 prime squares.
 * https://oeis.org/A105088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A105088 – Sum of the sides of ordered 2×2 prime squares
 * Formula: a(n) = 2 * (p(4n-3) + p(4n-2) + p(4n-1) + p(4n))
 * where p(k) is the k‑th prime (1‑based).
 * The implementation lazily constructs the list of primes needed for
 * a given index and returns the value using only integer arithmetic.
 */

#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Lazily generated list of primes */
/* prime_list[k] holds the (k+1)th prime (0‑based indexing). */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt  = 0;
static size_t   prime_cap  = 0;

/* Ensure that at least 'needed' primes are stored in prime_list. */
static void
ensure_primes(size_t needed)
{
    if (prime_cap == 0) {
        /* Initial capacity – will grow as needed. */
        prime_cap = 1024;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(*prime_list));
        if (!prime_list) exit(1);          /* malloc failure → abort */
    }
    while (prime_cnt < needed) {
        uint64_t candidate;
        if (prime_cnt == 0) {
            candidate = 2;
        } else {
            /* Next candidate is the prime after the current last one. */
            candidate = prime_list[prime_cnt - 1] + 1;
            /* Skip even numbers (except 2). */
            if (candidate > 2 && (candidate & 1) == 0)
                ++candidate;
        }

        /* Test primality of 'candidate'. */
        while (1) {
            int is_prime = 1;
            for (size_t i = 0; i < prime_cnt &&
                 prime_list[i] * prime_list[i] <= candidate; ++i) {
                if (candidate % prime_list[i] == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime)
                break;
            candidate += (candidate == 2 ? 1 : 2);  /* only odd steps */
        }

        /* Grow array if capacity reached. */
        if (prime_cnt == prime_cap) {
            prime_cap <<= 1; /* double the capacity */
            uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                prime_cap * sizeof(*prime_list));
            if (!tmp) exit(1);          /* realloc failure → abort */
            prime_list = tmp;
        }

        /* Store the found prime. */
        prime_list[prime_cnt++] = candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t
a(idx_t n)
{
    /* By OEIS definition, a(1) is the first term.  For n==0 we return 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Need the first prime of the block: p(4n-3).  Table indices 1‑based. */
    size_t needed = 4 * (size_t)n;
    ensure_primes(needed);

    /* Zero‑based index of the first of the four primes. */
    size_t idx = 4 * (size_t)n - 4;   /* 4n-4 == (4n-3)-1 */

    /* Sum the four consecutive primes.  All additions done in seq_t. */
    seq_t sum = 0;
    sum += (seq_t)prime_list[idx];
    sum += (seq_t)prime_list[idx + 1];
    sum += (seq_t)prime_list[idx + 2];
    sum += (seq_t)prime_list[idx + 3];

    /* a(n) is twice that sum. */
    return sum * (seq_t)2;
}
