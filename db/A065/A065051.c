/*
 * A065051  Let R(n) = n-th term of Recamán's sequence A005132; write R(n) =
 *          q*n + r with 0 <= r < n; sequence gives values of q.
 * https://oeis.org/A065051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(R(n)/n) where R(n) is the n-th Recamán term
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* static storage for the Recamán sequence, the quotient sequence, and a
   bit‑set of visited values                                            */
static seq_t *recam          = NULL; /* Recamán terms a[0],a[1],... */
static seq_t *quot           = NULL; /* quotients q[0],q[1],...  */
static size_t recam_cap      = 0;    /* allocated size of recam   */
static size_t recam_len      = 0;    /* number of terms stored    */
static size_t quot_cap       = 0;    /* allocated size of quot    */
static size_t quot_len       = 0;    /* number of quotients       */
static uint8_t *visited_bits = NULL;/* bitset for visited values */
static size_t visited_cap    = 0;    /* number of bytes in visited_bits */

/* mark a non‑negative integer as visited (assumes it fits in uint64_t) */
static void mark_visited(uint64_t v)
{
    size_t byte = v >> 3;
    if (byte >= visited_cap) {
        size_t new_cap = byte + 1;
        visited_bits = (uint8_t *)realloc(visited_bits, new_cap);
        memset(visited_bits + visited_cap, 0, new_cap - visited_cap);
        visited_cap = new_cap;
    }
    visited_bits[byte] |= (uint8_t)(1 << (v & 7));
}

/* test whether a non‑negative integer has been visited */
static int is_visited(uint64_t v)
{
    size_t byte = v >> 3;
    if (byte >= visited_cap) return 0;
    return (visited_bits[byte] >> (v & 7)) & 1;
}

/* ensure an array has at least cap_needed elements (power‑of‑two growth) */
static void ensure_capacity(seq_t **ptr, size_t *cap, size_t cap_needed)
{
    if (*cap >= cap_needed) return;
    size_t new_cap = (*cap == 0) ? 1 : *cap;
    while (new_cap < cap_needed) new_cap <<= 1;
    *ptr = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Returns the n-th term of OEIS A065051                                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* out of valid range */

    /* initialisation on first call */
    if (recam == NULL) {
        /* Recamán term a(0) = 0 */
        ensure_capacity(&recam, &recam_cap, 1);
        recam[0] = (seq_t)0;
        recam_len = 1;

        /* quotient array placeholder at index 0 */
        ensure_capacity(&quot, &quot_cap, 1);
        quot[0] = (seq_t)0;
        quot_len = 1;

        /* visited bit‑set: mark 0 as visited */
        visited_cap = 1;
        visited_bits = (uint8_t *)malloc(visited_cap);
        memset(visited_bits, 0, visited_cap);
        mark_visited(0);
    }

    /* compute terms up to index n */
    while (recam_len <= (size_t)n) {
        /* previous Recamán term and new index */
        seq_t prev  = recam[recam_len - 1];
        seq_t delta = (seq_t)recam_len;     /* i = n of the new term, 1‑based */

        /* decide whether to subtract or add */
        seq_t target;
        int use_target = 0;
        if (prev > delta) {
            target = prev - delta;
            if (!is_visited((uint64_t)target)) use_target = 1;
        }
        seq_t val = use_target ? target : prev + delta;

        /* store the new Recamán term and mark it */
        ensure_capacity(&recam, &recam_cap, recam_len + 1);
        recam[recam_len] = val;
        mark_visited((uint64_t)val);

        /* compute and store the quotient q = floor(val / delta) */
        ensure_capacity(&quot, &quot_cap, recam_len + 1);
        quot[recam_len] = val / delta;

        recam_len++;
    }

    return quot[n];
}
