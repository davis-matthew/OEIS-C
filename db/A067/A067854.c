/*
 * A067854  a(n) = cardinality of the (ordered) list L_n defined inductively
 *          by: L_1 = {2}; if, reading from left to right, there is a member p
 *          of L_n such that prime(n+1) can be appended to the beginning or
 *          end of p so that their neighboring digits are equal, then append
 *          prime(n+1) to the beginning or end o...
 * https://oeis.org/A067854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = cardinality of the ordered list Lₙ constructed from the
 *            first n primes as described in the OEIS entry
 * Range:     u8 n <= 409, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_stub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation does not rely on external utilities; it
   generates the required primes on the fly using a simple trial
   division algorithm.  The state is kept static so that calls to
   a() in any order are handled correctly. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_stub.h"

/* Types supplied by the build system */
extern seq_t a(idx_t n);
/* The function itself is defined below; the forward declaration is
   required only for the compiler; the actual definition follows. */

  /* unused stub */

/* ------------------------------------------------------------------ */
/* Helper: return the first decimal digit of a positive integer.      */
static inline uint8_t first_digit(idx_t v)
{
    while (v >= 10)
        v /= 10;
    return (uint8_t)v;
}

/* ------------------------------------------------------------------ */
/* Representation of an element of Lₙ – we only need its first and
   last decimal digits.                                                */
typedef struct {
    uint8_t first;
    uint8_t last;
} elem_t;

/* ------------------------------------------------------------------ */
/* Static state: maintained across all calls to a()                      */
static bool   initialized = false;

static idx_t *primes      = NULL;     /* 0‑based array of primes          */
static size_t primes_cnt  = 0;        /* number of primes generated        */
static uint8_t *prime_first = NULL;   /* first digit of each prime (0‑based)*/
static uint8_t *prime_last  = NULL;   /* last  digit of each prime (0‑based)*/

static elem_t *list      = NULL;      /* current list Lₙ                   */
static size_t  list_len  = 0;         /* number of elements in Lₙ          */
static size_t  list_cap  = 0;         /* allocated capacity for list       */

static seq_t *a_tab      = NULL;      /* a_tab[i] = a(i)                   */
static size_t  a_cap     = 0;         /* allocated capacity for a_tab      */
static idx_t   max_n_done = 0;        /* highest n for which we have a[n] */

/* ------------------------------------------------------------------ */
/* Generate primes up to at least 'needed_cnt' many primes.  This
   routine grows the prime array as required and keeps the per‑prime
   digit tables in sync.  It uses simple trial division – fast enough
   for the n ≲ 10⁴ range.                                            */
static void ensure_primes(size_t needed_cnt)
{
    if (primes_cnt >= needed_cnt)
        return;

    /* If no primes yet, start from 2, otherwise start just above the
       largest prime we already have. */
    idx_t candidate = primes_cnt == 0 ? 2 : primes[primes_cnt - 1] + 1;

    /* Reserve a bit of extra space to avoid frequent reallocs. */
    size_t new_cap = primes_cnt > 0 ? primes_cnt * 2 : 64;
    if (new_cap < needed_cnt)
        new_cap = needed_cnt;

    primes = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
    prime_first = (uint8_t *)realloc(prime_first, new_cap * sizeof(uint8_t));
    prime_last  = (uint8_t *)realloc(prime_last,  new_cap * sizeof(uint8_t));

    while (primes_cnt < needed_cnt) {
        bool is_prime = true;
        idx_t limit = (idx_t)(candidate * candidate <= UINT64_MAX ? candidate : 0);
        for (size_t i = 0; i < primes_cnt; ++i) {
            idx_t p = primes[i];
            if (p > limit)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            primes[primes_cnt] = candidate;
            prime_first[primes_cnt] = first_digit(candidate);
            prime_last[primes_cnt]  = (uint8_t)(candidate % 10);
            ++primes_cnt;
            if (primes_cnt == new_cap) {
                new_cap <<= 1;
                primes = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
                prime_first = (uint8_t *)realloc(prime_first, new_cap * sizeof(uint8_t));
                prime_last  = (uint8_t *)realloc(prime_last,  new_cap * sizeof(uint8_t));
            }
        }
        ++candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Initialise static arrays for L₁ and a(1).  This is done lazily on
   the first call to a(n).                                             */
static void init_static(void)
{
    if (initialized)
        return;

    /* Generate at least the first prime. */
    ensure_primes(1);

    /* Start with L₁ = {2}   – its first and last digits are both 2. */
    list_cap = 16;
    list = (elem_t *)malloc(list_cap * sizeof(elem_t));
    list[0].first = prime_first[0];
    list[0].last  = prime_last[0];
    list_len = 1;

    /* Allocate a_tab for a(1). */
    a_cap = 16;
    a_tab = (seq_t *)malloc(a_cap * sizeof(seq_t));
    a_tab[0] = 0;          /* a(0) is unused */
    a_tab[1] = (seq_t)1;   /* a(1) = 1 */
    max_n_done = 1;

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Extend the computation up to n (inclusive).  For each new n we
   consider prime number p_{n-1} (0‑based index n-1).  The rule
   described in the OEIS entry is applied to grow Lₙ.               */
static void ensure_a(idx_t n)
{
    init_static();
    if (n <= max_n_done)
        return;

    ensure_primes(n);  /* we need the first n primes */

    for (idx_t curr = max_n_done + 1; curr <= n; ++curr) {
        idx_t pidx = curr - 1;           /* 0‑based index of the prime to use */
        uint8_t q_first = prime_first[pidx];
        uint8_t q_last  = prime_last[pidx];

        bool merged = false;

        for (size_t i = 0; i < list_len; ++i) {
            if (q_last == list[i].first) {          /* prepend q */
                list[i].first = q_first;
                merged = true;
                break;
            }
            if (q_first == list[i].last) {           /* append q */
                list[i].last = q_last;
                merged = true;
                break;
            }
        }

        if (!merged) {            /* no match – create new element */
            if (list_len == list_cap) {
                list_cap <<= 1;
                list = (elem_t *)realloc(list, list_cap * sizeof(elem_t));
            }
            list[list_len].first = q_first;
            list[list_len].last  = q_last;
            ++list_len;
        }

        /* store a(curr) = current size of L_ curr */
        if (curr >= (idx_t)a_cap) {
            a_cap <<= 1;
            a_tab = (seq_t *)realloc(a_tab, a_cap * sizeof(seq_t));
        }
        a_tab[curr] = (seq_t)list_len;
    }
    max_n_done = n;
}

/* ------------------------------------------------------------------ */
/* Public function required by the specification.                      */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined by the OEIS description   */
    ensure_a(n);
    return a_tab[n];
}
