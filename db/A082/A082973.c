/*
 * A082973  Fibonacci numbers which are of the form Sum{i=1..k} prime(i) for
 *          some k.
 * https://oeis.org/A082973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, bref
 * Formula:   a(n) = the n-th Fibonacci number that equals the sum of the
 *            first k primes for some k. Computation by simultaneous scanning
 *            of Fibonacci numbers and cumulative prime sums
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Validated: partial, pass, 3 terms through n = 2 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static seq_t *term_array   = NULL;  /* stored matching Fibonacci numbers */
static size_t term_cap    = 0;      /* current capacity of term_array */
static size_t term_cnt    = 0;      /* number of terms already found */

static idx_t   prime_next = 1;      /* next prime index to add to the sum */
static seq_t   prime_sum  = 0;      /* cumulative sum of primes */

static seq_t   fib_prev = 0;        /* Fibonacci(n-1) */
static seq_t   fib_curr = 1;        /* Fibonacci(n)   */
static idx_t   fib_idx  = 1;        /* current Fibonacci index (fib_curr) */

/* Ensure term_array can hold at least 'needed' elements. */
static void
ensure_capacity(size_t needed)
{
    if (needed <= term_cap)
        return;
    size_t new_cap = term_cap ? term_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *p = mem_resize(seq_t, term_array, new_cap);
    term_array = p;
    term_cap   = new_cap;
}

/* Store a new term into term_array. */
static void
add_term(seq_t val)
{
    ensure_capacity(term_cnt + 1);
    term_array[term_cnt++] = val;
}

/* The required OEIS sequence function. */
seq_t
a(idx_t n)
{
    /* Initialise static state on first call. */
    if (term_array == NULL) {
        term_array = NULL;
        term_cap   = 0;
        term_cnt   = 0;
        prime_next = 1;
        prime_sum  = 0;
        fib_prev   = 0;
        fib_curr   = 1;
        fib_idx    = 1;
    }

    /* Generate terms until we reach the desired index n. */
    while (term_cnt <= n) {
        /* Raise the prime sum to at least the current Fibonacci value. */
        while (prime_sum < fib_curr) {
            seq_t p = (seq_t)get_prime(prime_next);
            prime_sum += p;
            prime_next++;
        }

        /* If they match, record the Fibonacci number. */
        if (prime_sum == fib_curr) {
            add_term(fib_curr);
        }

        /* Advance to the next Fibonacci number. */
        seq_t next_val = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = next_val;
        fib_idx++;
    }

    return term_array[n];
}
