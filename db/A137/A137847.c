/*
 * A137847  Smallest number missing in the EKG sequence (A064413) after n
 *          terms.
 * https://oeis.org/A137847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest positive integer that is not among the first
 *            n terms of the EKG sequence (A064413). The EKG sequence starts
 *            with 1 and subsequently chooses the smallest unused integer that
 *            shares a common factor > 1 with the previous term. This
 *            implementation builds the sequence lazily, keeps...
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* ---------- global state ------------------------------------------ */
static seq_t *ekg_seq  = NULL;  /* array of EKG terms */
static size_t ekg_len  = 0;     /* number of terms stored */
static size_t ekg_cap  = 0;     /* allocated capacity */

static uint8_t *used    = NULL;       /* bitmap: used[i] != 0 iff i appears in the sequence */
static size_t used_size = 0;          /* bitmap capacity */

/* ---------- small utilities --------------------------------------- */

/* Euclidean algorithm on seq_t values. */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Ensure the EKG array can hold at least 'cap' terms. */
static void ensure_ekg_capacity(size_t cap)
{
    if (ekg_cap >= cap) return;
    size_t newcap = ekg_cap ? ekg_cap : 16;
    while (newcap < cap) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(ekg_seq, newcap * sizeof(seq_t));
    if (!tmp) abort();
    ekg_seq = tmp;
    ekg_cap = newcap;
}

/* Ensure the bitmap can index up to 'idx' (inclusive). */
static void ensure_used_capacity(size_t idx)
{
    if (used_size > idx) return;
    size_t newsize = used_size ? used_size : 64;
    while (newsize <= idx) newsize <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used, newsize * sizeof(uint8_t));
    if (!tmp) abort();
    if (used_size < newsize) {
        memset(tmp + used_size, 0, newsize - used_size);
    }
    used = tmp;
    used_size = newsize;
}

/* ---------- generate EKG terms up to n (n>=1) ---------------------- */

static void ensure_terms(idx_t n)
{
    if (ekg_len >= (size_t)n) return;          /* already ready */

    /* initialise state on first call */
    if (ekg_seq == NULL) {
        ekg_cap = 16;
        ekg_seq = (seq_t *)malloc(ekg_cap * sizeof(seq_t));
        if (!ekg_seq) abort();
        ekg_len = 1;
        ekg_seq[0] = (seq_t)1;                 /* a(1) = 1 */

        used_size = 64;
        used = (uint8_t *)calloc(used_size, sizeof(uint8_t));
        if (!used) abort();
        used[1] = 1;                           /* mark 1 as used */
    }

    while (ekg_len < (size_t)n) {
        seq_t val;
        if (ekg_len == 1) {
            val = (seq_t)2;                    /* second term is 2 */
        } else {
            seq_t prev = ekg_seq[ekg_len - 1];
            seq_t cand = (seq_t)2;
            for (;;) {
                ensure_used_capacity((size_t)cand);
                if (!used[(size_t)cand] && seq_gcd(prev, cand) > (seq_t)1)
                    break;
                ++cand;
            }
            val = cand;
        }

        /* append the new term */
        ensure_ekg_capacity(ekg_len + 1);
        ekg_seq[ekg_len++] = val;

        /* mark as used */
        ensure_used_capacity((size_t)val);
        used[(size_t)val] = 1;
    }
}

/* ---------- find the smallest missing integer --------------------- */

static seq_t compute_missing(void)
{
    size_t m = 2;            /* 1 is always used */
    for (;;) {
        ensure_used_capacity(m);
        if (!used[m]) break;
        ++m;
    }
    return (seq_t)m;
}

/* ---------- public routine ---------------------------------------- */

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for n < 1 */
    ensure_terms(n);
    return compute_missing();
}
