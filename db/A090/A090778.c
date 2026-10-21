/*
 * A090778  Numbers k such that phi(k) divides k*(k - phi(k)).
 * https://oeis.org/A090778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 1629, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This function returns the n-th term of OEIS sequence A090778:
 * numbers k such that φ(k) divides k*(k - φ(k)).
 *
 * It uses a static φ-sieve and stores all qualifying k in a static array.
 * Calls in any order are handled by dynamic re‑allocation.
 */
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
  /* In C++ use new[] / delete[], in C use malloc()/free() */
  #define NEW_ARRAY(T, n) new T[(n)]
  #define DELETE_ARRAY(p) delete[] (p)
#else
  #include <stdlib.h>
  #define NEW_ARRAY(T, n) ((T*)malloc((n)*sizeof(T)))
  #define DELETE_ARRAY(p) free(p)
#endif

/* static storage for the sequence values             */
static seq_t *seq      = NULL;
static size_t seq_cap  = 0;
static size_t seq_cnt  = 0;

/* static storage for φ values (uint64_t is enough for the required range) */
static uint64_t *phi    = NULL;
static size_t phi_cap   = 0;
static size_t checked_up_to = 0;

/* append a value to the sequence array */
static void append_value(seq_t v)
{
    if (seq_cnt == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_t *new_arr = NEW_ARRAY(seq_t, new_cap);
        for (size_t i = 0; i < seq_cnt; ++i)
            new_arr[i] = seq[i];
        if (seq) DELETE_ARRAY(seq);
        seq = new_arr;
        seq_cap = new_cap;
    }
    seq[seq_cnt++] = v;
}

/* extend the sieve to cover all k ≤ limit and test new numbers */
static void extend_to(size_t limit)
{
    if (limit <= checked_up_to) return;

    /* Resize phi array if necessary */
    if (limit + 1 > phi_cap) {
        uint64_t *new_phi = NEW_ARRAY(uint64_t, limit + 1);
        for (size_t i = 0; i <= limit; ++i)
            new_phi[i] = i;
        if (phi) DELETE_ARRAY(phi);
        phi   = new_phi;
        phi_cap = limit + 1;
    } else {
        /* initialize only the new entries */
        for (size_t i = checked_up_to + 1; i <= limit; ++i)
            phi[i] = i;
    }

    /* Sieve to compute φ values */
    for (size_t i = 2; i <= limit; ++i) {
        if (phi[i] == i) {            /* i is prime */
            uint64_t p = i;
            for (size_t j = i; j <= limit; j += i)
                phi[j] -= phi[j] / p;
        }
    }

    /* Test new k and record those that satisfy the condition */
    for (size_t i = checked_up_to + 1; i <= limit; ++i) {
        seq_t k      = (seq_t)i;
        seq_t phi_k  = (seq_t)phi[i];
        seq_t prod   = k * (k - phi_k);
        if (phi_k != 0 && (prod % phi_k) == 0)
            append_value(k);
    }

    checked_up_to = limit;
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but harmless */

    /* Ensure we have at least n terms generated */
    while (seq_cnt < (size_t)n) {
        size_t new_limit = checked_up_to ? checked_up_to * 2 : 8;
        extend_to(new_limit);
    }

    return seq[(size_t)n - 1];            /* a(1) is seq[0] */
}
