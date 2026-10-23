/*
 * A109738  a(0)=1; for n>0, a(n) = a(n-1) S prime(n), where S is + if n is
 *          odd and a(n-1) is even, x if n is odd and a(n-1) is odd, - if n is
 *          even and a(n-1) is even, + if n is even and a(n-1) is odd.
 * https://oeis.org/A109738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; for n > 0, a(n) = a(n‑1) ∘ prime(n) where ∘ = '+' if n
 *            odd & a(n‑1) even, '×' if n odd & a(n‑1) odd, '−' if n even &
 *            a(n‑1) even, '+' if n even & a(n‑1) odd. (prime(1) = 2, prime(2)
 *            = 3, …)
 * Range:     i8 n <= 10, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*--------------------------  dynamic array for the sequence  --------------------------*/
static seq_t *seq_arr  = NULL;   /* a(0)…a(seq_size‑1)  */
static size_t seq_cap  = 0;      /* allocated capacity   */
static size_t seq_size = 0;      /* number of values filled */

/* enlarge the sequence array if necessary (n is 0‑based) */
static void ensure_seq_capacity(size_t n)
{
    if (seq_cap > n) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap <= n) new_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/*--------------------------------  prime generator  ---------------------------------*/
static uint64_t *prime_arr = NULL;   /* primes[0] = 2, . . . */
static size_t   prime_cap = 0;
static size_t   prime_size = 0;      /* number of primes stored */

/* make sure we have at least `count` primes (count ≥ 1) */
static void ensure_prime_capacity(size_t count)
{
    /* candidate starts just above the last stored prime */
    uint64_t candidate = (prime_size == 0) ? 2 : prime_arr[prime_size-1] + 1;

    while (prime_size < count) {
        bool is_p = true;
        /* simple trial division using stored primes */
        for (size_t i = 0; i < prime_size && prime_arr[i]*prime_arr[i] <= candidate; ++i) {
            if (candidate % prime_arr[i] == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (prime_size >= prime_cap) {
                prime_cap = prime_cap ? prime_cap << 1 : 8;
                prime_arr = (uint64_t *)realloc(prime_arr, prime_cap * sizeof(uint64_t));
            }
            prime_arr[prime_size++] = candidate;
            candidate = prime_arr[prime_size-1] + 1;   /* next candidate starts after the new prime */
        } else {
            ++candidate;                                 /* try the next integer */
        }
    }
}

/*------------------------------  sequence function  --------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    size_t idx = (size_t)n;
    ensure_seq_capacity(idx);

    /* initialise a(0) if not yet done */
    if (seq_size == 0) {
        seq_arr[0] = (seq_t)1;
        seq_size  = 1;
    }

    /* we need the first (idx+1) primes, i.e. prime(idx) */
    ensure_prime_capacity(idx);

    for (size_t i = seq_size; i <= idx; ++i) {
        seq_t prev = seq_arr[i-1];
        uint64_t p  = prime_arr[i-1];          /* prime(i) */
        bool i_odd      = (i & 1);
        bool prev_even  = ((prev % (seq_t)2) == 0);

        seq_t cur;
        if (i_odd) {
            if (prev_even) cur = prev + (seq_t)p;
            else           cur = prev * (seq_t)p;
        } else { /* i even */
            if (prev_even) cur = prev - (seq_t)p;
            else           cur = prev + (seq_t)p;
        }
        seq_arr[i] = cur;
    }

    seq_size = idx + 1;
    return seq_arr[idx];
}
