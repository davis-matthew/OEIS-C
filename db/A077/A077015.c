/*
 * A077015  a(1) = 1, continue by multiplying by the smallest composite number
 *          and then dividing by the smallest prime not used earlier and
 *          taking the integer part at every step.
 * https://oeis.org/A077015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A077015:
   a(1) = 1;
   Let c_k be the k‑th composite number (4, 6, 8, 9, …),
   and p_k the k‑th prime (2, 3, 5, 7, 11, …).
   For n ≥ 2:
      if n is even:   a(n) = a(n‑1) * c_{n/2}
      else:           a(n) = floor( a(n‑1) / p_{(n‑1)/2} )
   All arithmetic is carried out in the type seq_t. */

#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------
   Static cache for computed values of the sequence
   -------------------------------------------------------------- */
static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;   /* number of stored terms           */
static size_t a_cache_cap  = 0;   /* capacity of a_cache              */

/* --------------------------------------------------------------
   Prime and composite number lists
   -------------------------------------------------------------- */
static uint64_t *prime_list     = NULL;
static size_t    prime_count    = 0;
static size_t    prime_cap      = 0;

static uint64_t *composite_list = NULL;
static size_t    composite_count = 0;
static size_t    composite_cap   = 0;

/* --------------------------------------------------------------
   current number inspected for primality/compositeness
   -------------------------------------------------------------- */
static uint64_t last_checked = 3;   /* numbers ≤ 3 are known */

static void add_prime(uint64_t p)
{
    if (prime_count == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        prime_cap = new_cap;
    }
    prime_list[prime_count++] = p;
}

static void add_composite(uint64_t c)
{
    if (c < 4) return;
    if (composite_count == composite_cap) {
        size_t new_cap = composite_cap ? composite_cap * 2 : 8;
        uint64_t *tmp = (uint64_t *)realloc(composite_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        composite_list = tmp;
        composite_cap = new_cap;
    }
    composite_list[composite_count++] = c;
}

/* Pre‑seed the prime list with 2 and 3 */
static void init_prime_list(void)
{
    if (prime_count == 0) {
        /* allocate enough space for at least 2 primes */
        prime_cap = 8;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) exit(EXIT_FAILURE);
        add_prime(2);
        add_prime(3);
    }
}

/* Generate enough primes and composites to satisfy the request.
   need_primes   = number of primes required (1‑based count)
   need_comps    = number of composites required (1‑based count) */
static void ensure_pm(size_t need_primes, size_t need_comps)
{
    init_prime_list();

    if (prime_count >= need_primes && composite_count >= need_comps) return;

    /* Extend from last_checked+1 upwards */
    while (prime_count < need_primes || composite_count < need_comps) {
        ++last_checked;
        int is_prime = 1;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > last_checked) break;
            if (last_checked % p == 0) { is_prime = 0; break; }
        }
        if (is_prime)
            add_prime(last_checked);
        else
            add_composite(last_checked);
    }
}

/* Append a value to the a_cache */
static void append_value(seq_t v)
{
    if (a_cache_size == a_cache_cap) {
        size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        a_cache = tmp;
        a_cache_cap = new_cap;
    }
    a_cache[a_cache_size++] = v;
}

/* Main function computing the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined slot, return 0 */

    /* Initialise cache with the first term if empty */
    if (a_cache_size == 0) {
        a_cache_cap = 8;
        a_cache = (seq_t *)malloc(a_cache_cap * sizeof(seq_t));
        if (!a_cache) exit(EXIT_FAILURE);
        a_cache[0] = (seq_t)1;   /* a(1) = 1 */
        a_cache_size = 1;
    }

    /* Return cached value if already computed */
    if (n <= a_cache_size) return a_cache[n-1];

    /* Ensure enough primes and composites have been generated */
    size_t need_primes = (size_t)((n - 1) / 2);   /* primes used up to n-1 */
    size_t need_comps  = (size_t)(n / 2);         /* composites used up to n */
    ensure_pm(need_primes, need_comps);

    /* Compute missing terms one by one */
    for (idx_t i = a_cache_size; i < n; ++i) {
        seq_t prev = a_cache[i-1];
        idx_t term_index = i + 1;   /* actual n index (starting at 1) */

        if ((term_index & 1ULL) == 0ULL) {          /* even n: multiply by composite */
            size_t comp_idx = (size_t)term_index / 2; /* 1‑based index into composite_list */
            seq_t c = (seq_t)composite_list[comp_idx - 1];
            append_value(prev * c);
        } else {                                     /* odd n: divide by prime */
            size_t prime_idx = (size_t)term_index / 2;/* floor((n-1)/2) in 1‑based */
            seq_t p = (seq_t)prime_list[prime_idx - 1];
            append_value(prev / p);
        }
    }
    return a_cache[n-1];
}
