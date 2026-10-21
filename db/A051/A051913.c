/*
 * A051913  Numbers k such that phi(k)/phi(phi(k)) = 3.
 * https://oeis.org/A051913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051913: numbers k such that  φ(k) = 2^a * 3^b  (a,b>0) */
/* The function generates the sequence by sieve‑based Euler totient
   computation and keeps all k whose totient has only the prime factors
   2 and 3 and contains both.  It is memoised so any order of calls
   works and it runs in O(n log log n). */

#include <stdlib.h>

typedef struct {
    /* cached terms of the sequence */
    seq_t *cache;
    size_t cache_cap;
    size_t cache_size;

    /* next integer to test */
    idx_t next;

    /* φ values up to limit */
    seq_t *phi;
    idx_t *lp;
    idx_t limit;
} seq_state_t;

/* Resize φ and lp arrays to at least new_limit and recompute all φ
   values from scratch.  The arrays are reallocated (not sliced). */
static void recompute_phi(seq_state_t *st, idx_t new_limit) {
    if (new_limit <= st->limit)
        return;

    st->phi = (seq_t *)realloc(st->phi, (new_limit + 1) * sizeof(seq_t));
    st->lp  = (idx_t *)realloc(st->lp,  (new_limit + 1) * sizeof(idx_t));

    /* Linear sieve up to new_limit */
    size_t sz = (size_t)new_limit + 1;
    size_t prime_cnt = 0;
    idx_t *prime_list = (idx_t *)malloc(sz * sizeof(idx_t));

    st->phi[0] = (seq_t)0;
    st->phi[1] = (seq_t)1;
    for (size_t i = 2; i < sz; ++i) {
        st->lp[i] = 0;
    }

    for (idx_t i = 2; i <= new_limit; ++i) {
        if (st->lp[i] == 0) {                    /* i is prime */
            st->lp[i] = i;
            prime_list[prime_cnt++] = i;
            st->phi[i] = (seq_t)(i - 1);          /* φ(p) = p-1      */
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            idx_t p = prime_list[j];
            if (p > st->lp[i] || i * p > new_limit)
                break;
            st->lp[i * p] = p;
            if (i % p == 0) {
                st->phi[i * p] = st->phi[i] * (seq_t)p;
            } else {
                st->phi[i * p] = st->phi[i] * ((seq_t)p - 1);
            }
        }
    }

    free(prime_list);
    st->limit = new_limit;
}

/* Ensure the cache has room for at least need terms. */
static void ensure_cache_capacity(seq_state_t *st, size_t need) {
    if (need <= st->cache_cap)
        return;
    size_t new_cap = (st->cache_cap == 0) ? 8 : st->cache_cap * 2;
    if (new_cap < need)
        new_cap = need;
    st->cache = (seq_t *)realloc(st->cache, new_cap * sizeof(seq_t));
    st->cache_cap = new_cap;
}

/* ----- public sequence function ----- */
seq_t a(idx_t n) {
    static seq_state_t st = {0};

    /* one‑time initialisation */
    if (st.cache == NULL) {
        st.next = 2;            /* start testing from 2 */
        st.cache_size = 0;
        st.cache_cap = 0;
        st.cache = NULL;
        st.phi = NULL;
        st.lp = NULL;
        st.limit = 0;
    }

    /* generate terms until we have at least n */
    while (st.cache_size < n) {
        /* make sure φ is computed up to the current integer */
        if (st.next > st.limit) {
            idx_t new_limit = (st.limit == 0) ? 10 : st.limit * 2 + 1;
            recompute_phi(&st, new_limit);
        }

        seq_t phi_val = st.phi[(size_t)st.next];

        /* check if phi_val = 2^a * 3^b with a,b>0 */
        if ((phi_val & 1) == 0) {            /* divisible by 2 */
            seq_t t = phi_val;
            while ((t & 1) == 0)             /* strip 2s */
                t >>= 1;
            if (t % 3 == 0) {                /* at least one 3 */
                while (t % 3 == 0)
                    t /= 3;                  /* strip 3s */
                if (t == 1) {                /* no other primes */
                    ensure_cache_capacity(&st, st.cache_size + 1);
                    st.cache[st.cache_size++] = (seq_t)st.next;
                }
            }
        }

        ++st.next;
    }

    return st.cache[n - 1];
}
