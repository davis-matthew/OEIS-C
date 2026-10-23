/*
 * A109334  Integers with mutual residues of 9 or more.
 * https://oeis.org/A109334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A109334: integers with mutual residues of 9 or more.
    a(1) = 10; for n>1, a(n) = min{ m > a(n‑1) |  m mod a(i) >= 9  for all i < n }.
    The algorithm stores all previously found terms and, for each new term,
    iteratively raises a candidate until it satisfies the remainder condition
    for every stored divisor.  */
#include <stdlib.h>
#include <stdbool.h>

/* Storage for computed sequence terms.  seq[0] corresponds to a(1). */
static seq_t *seq      = NULL;   /* array of terms */
static idx_t  seq_size = 0;      /* number of terms currently stored */
static idx_t  seq_cap  = 0;      /* allocated capacity of seq */

/* Remainders of the current candidate modulo each stored term. */
static seq_t *rema     = NULL;   /* array of remainders */
static idx_t  rema_cap = 0;      /* allocated capacity of rema */

/* Ensure that seq has capacity for at least 'need' terms. */
static void
ensure_seq_cap(idx_t need)
{
    if (need <= seq_cap)
        return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure that rema has capacity for at least 'need' remainders. */
static void
ensure_rema_cap(idx_t need)
{
    if (need <= rema_cap)
        return;
    idx_t new_cap = rema_cap ? rema_cap << 1 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    rema = (seq_t *)realloc(rema, new_cap * sizeof(seq_t));
    rema_cap = new_cap;
}

/* Main routine returning the nth term (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)   /* sequence is 1‑based; undefined for n==0 */
        return (seq_t)0;

    /* Initialise the first term lazily. */
    if (seq_size == 0) {
        ensure_seq_cap(1);
        seq[0] = (seq_t)10;   /* a(1) = 10 */
        seq_size = 1;
        if (n == 1)
            return seq[0];
    }

    /* Compute terms up to index n. */
    while (seq_size < n) {
        /* Prepare for the next term. */
        idx_t prev_idx = seq_size - 1;
        seq_t prev      = seq[prev_idx];
        seq_t candidate = prev + (seq_t)1;

        /* Ensure rema array large enough for all previous terms. */
        ensure_rema_cap(seq_size);

        /* Compute initial remainders of candidate modulo each prior term. */
        for (idx_t i = 0; i < seq_size; ++i)
            rema[i] = candidate % seq[i];

        /* Iteratively increase candidate until all remainders ≥ 9. */
        while (1) {
            bool ok = true;
            for (idx_t i = 0; i < seq_size; ++i) {
                if (rema[i] < (seq_t)9) {
                    seq_t delta = (seq_t)9 - rema[i];   /* 0 < delta < 9 */
                    candidate += delta;

                    /* Update all remainders efficiently. */
                    for (idx_t j = 0; j < seq_size; ++j) {
                        seq_t tmp = rema[j] + delta;
                        if (tmp >= seq[j])
                            tmp -= seq[j];
                        rema[j] = tmp;
                    }
                    ok = false;
                    break; /* restart checking with updated candidate */
                }
            }
            if (ok)
                break;
        }

        /* Store the newly found term. */
        ensure_seq_cap(seq_size + 1);
        seq[seq_size++] = candidate;
    }

    return seq[n - 1];
}
