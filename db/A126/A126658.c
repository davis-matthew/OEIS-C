/*
 * A126658  Prime numbers that are the sum of three distinct positive eighth
 *          powers.
 * https://oeis.org/A126658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime that can be expressed as the sum of three
 *            distinct positive eighth powers
 * Range:     u8 none, u16 none, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-03)
 * Helpers:   ipow_idx, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
/* Utility headers */
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Static state to allow out‑of‑order calls. */
    static idx_t   max_c = 0;             /* largest c processed */
    static seq_t *powers = NULL;          /* powers[1..max_c] = i^8   */
    static size_t powers_cap = 0;         /* capacity of powers       */
    static seq_t *prime_array = NULL;    /* all primes found (maybe unsorted) */
    static size_t prime_len = 0;          /* current number of elements    */
    static size_t prime_cap = 0;          /* capacity of prime_array       */

    /* --------------------------------------------------------------------
       Generate new triples a<b<c until we have at least n primes (unsorted).
       -------------------------------------------------------------------- */
    while (prime_len < n) {
        /* Increase c and compute its eighth power */
        ++max_c;
        if (max_c >= powers_cap) {
            size_t new_cap = (powers_cap == 0) ? 4 : powers_cap * 2;
            if (new_cap < max_c + 1) new_cap = max_c + 1;
            powers = seq_realloc(powers, powers_cap, new_cap);
            powers_cap = new_cap;
        }
        powers[max_c] = ipow_idx((seq_t)max_c, (idx_t)8);

        /* Enumerate all distinct pairs (a,b) with a<b<c */
        for (idx_t a = 1; a + 2 <= max_c; ++a) {
            seq_t pa = powers[a];
            for (idx_t b = a + 1; b + 1 <= max_c; ++b) {
                seq_t sum = pa + powers[b] + powers[max_c];
                if (is_prime_big(sum))
                    append_candidate(&prime_array, &prime_len, &prime_cap, sum);
            }
        }
    }

    /* --------------------------------------------------------------------
       After generating enough primes (unsorted) we sort and dedupe.
       This may be repeated if duplicates were removed and more primes
       are still needed.
       -------------------------------------------------------------------- */
    while (true) {
        /* Sort the array and remove duplicates */
        mem_qsort(prime_array, prime_len, sizeof(seq_t), cmp_seq_t);

        size_t uniq = 0;
        for (size_t i = 0; i < prime_len; ++i) {
            if (uniq == 0 || cmp_seq_t(&prime_array[i],
                                      &prime_array[uniq - 1]) != 0) {
                prime_array[uniq++] = prime_array[i];
            }
        }
        prime_len = uniq;   /* now prime_len is the number of unique primes */

        if (prime_len >= n)
            break;          /* enough primes have been found */
        /* Otherwise, generate more triples and repeat */
        while (prime_len < n) {
            ++max_c;
            if (max_c >= powers_cap) {
                size_t new_cap = (powers_cap == 0) ? 4 : powers_cap * 2;
                if (new_cap < max_c + 1) new_cap = max_c + 1;
                powers = seq_realloc(powers, powers_cap, new_cap);
                powers_cap = new_cap;
            }
            powers[max_c] = ipow_idx((seq_t)max_c, (idx_t)8);

            for (idx_t a = 1; a + 2 <= max_c; ++a) {
                seq_t pa = powers[a];
                for (idx_t b = a + 1; b + 1 <= max_c; ++b) {
                    seq_t sum = pa + powers[b] + powers[max_c];
                    if (is_prime_big(sum))
                        append_candidate(&prime_array, &prime_len, &prime_cap, sum);
                }
            }
        }
    }

    return prime_array[n - 1];
}
