/*
 * A027700  Numbers k such that the k-th prime has an even number of 1's in
 *          its binary expansion.
 * https://oeis.org/A027700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A027700: Numbers k such that the k‑th prime has an even number of 1's
   in its binary representation. The sequence function a(n) builds primes
   incrementally and keeps the indices whose primes satisfy the parity
   condition. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*--------------------------------------------------------------------*/
/* Dynamic arrays for primes and qualifying k-values.                 */
/*--------------------------------------------------------------------*/
static uint64_t *prime_list   = NULL;   /* primes found so far */
static size_t   prime_count   = 0;
static size_t   prime_capacity = 0;

static seq_t *k_list        = NULL;   /* k-values with even popcount */
static size_t  k_count      = 0;
static size_t  k_capacity   = 0;

/* Next odd integer to test for primality.  2 is handled separately. */
static uint64_t next_candidate = 3;

/* Grow a dynamic array.  `capacity` points to current capacity.       */
static void grow_array(size_t *capacity, void **ptr, size_t elem_size)
{
    size_t new_cap = *capacity ? (*capacity) * 2 : 8;
    void *res = realloc(*ptr, new_cap * elem_size);
    if (!res) abort();
    *ptr = res;
    *capacity = new_cap;
}

/* Push a new prime into prime_list.                                     */
static void push_prime(uint64_t p)
{
    if (prime_count == prime_capacity)
        grow_array(&prime_capacity, (void **)&prime_list, sizeof(uint64_t));
    prime_list[prime_count++] = p;
}

/* Push a new qualifying k-value into k_list.                           */
static void push_k(size_t k)
{
    if (k_count == k_capacity)
        grow_array(&k_capacity, (void **)&k_list, sizeof(seq_t));
    k_list[k_count++] = (seq_t)k;   /* cast; overflow is fine for small seq_t */
}

/* Return 1 if the popcount of x is even, 0 otherwise.                 */
static int popcount_even(uint64_t x)
{
    int parity = 0;
    while (x) {
        parity ^= 1;
        x &= x - 1;          /* drop lowest set bit */
    }
    return parity == 0;
}

/* Ensure that at least `need` qualifying k-values have been generated. */
static void ensure_k_size(idx_t need)
{
    if (need == 0) return;

    /* Start with the first prime, 2.                                  */
    if (prime_count == 0) {
        push_prime(2);            /* prime 2 has odd popcount, so not stored in k_list */
    }

    while (k_count < (size_t)need) {
        uint64_t p = next_candidate;

        /* Trial division by known primes up to sqrt(p). */
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t pr = prime_list[i];
            if (pr * pr > p) break;
            if (p % pr == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            push_prime(p);
            size_t k_index = prime_count;               /* 1‑based index of this prime */
            if (popcount_even(p))
                push_k(k_index);
        }

        next_candidate += 2;      /* next odd candidate */
    }
}

/*--------------------------------------------------------------------*/
/* Sequence function a(idx_t n).  n>=1.                                 */
/*--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */
    ensure_k_size(n);
    return k_list[n - 1];
}
