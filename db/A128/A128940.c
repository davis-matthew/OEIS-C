/*
 * A128940  List of triples of primes with common difference 6.
 * https://oeis.org/A128940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 698, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: list of primes p such that p, p+6, p+12 are all prime
   (prime triples).  The sequence enumerates each prime of each triple
   in order of appearance.  Only triples with all primes > 43 are
   considered, as required by OEIS A128940. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/build_primes_upto.h"

/* --- Static state ------------------------------------------------------- */
static uint64_t *small_primes = NULL;   /* primes up to a fixed limit */
static size_t   small_prime_count = 0;  /* number of small primes          */
static bool     primes_inited = false;

static seq_t *output_seq   = NULL;      /* generated sequence elements    */
static size_t output_len   = 0;         /* number of generated elements    */
static size_t output_cap   = 0;         /* capacity of output_seq          */

static uint64_t seg_low      = 2;       /* start of current segment       */
static uint64_t seg_high     = 0;       /* end (exclusive) of current seg */
static const uint64_t SEGMENT_SIZE = (1ULL << 20); /* 1 048 576 */

static uint64_t prev_prime   = 0;       /* previous prime seen            */
static int       run_len     = 0;       /* length of current diff‑6 run   */
static seq_t    run_buf[3];            /* last three primes of run       */

/* ---------------------------------------------------------------------- */
/* initialise the table of small primes (up to 1 000 000) */
static void init_small_primes(void)
{
    if (primes_inited)
        return;
    const uint64_t LIMIT = 1000000ULL;
    size_t cnt;
    build_primes_upto(LIMIT, &small_primes, &cnt);
    small_prime_count = cnt;
    primes_inited = true;
}

/* ---------------------------------------------------------------------- */
/* ensure that the output array has at least 'need' slots                */
static void ensure_output_capacity(size_t need)
{
    if (need <= output_cap)
        return;
    size_t new_cap = output_cap ? output_cap * 2 : 128;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(output_seq, new_cap * sizeof(seq_t));
    output_seq = tmp;
    output_cap = new_cap;
}

/* ---------------------------------------------------------------------- */
/* process a new prime and update the run tracking.                        */
static void process_prime(uint64_t p)
{
    if (prev_prime == 0) {          /* first prime encountered */
        run_buf[0] = (seq_t)p;
        run_len = 1;
    } else {
        uint64_t diff = p - prev_prime;
        if (diff == 6ULL) {         /* still a 6‑gap run          */
            if (run_len < 3) {
                run_buf[run_len++] = (seq_t)p;
            } else {                /* rotate the buffer           */
                run_buf[0] = run_buf[1];
                run_buf[1] = run_buf[2];
                run_buf[2] = (seq_t)p;
            }
            if (run_len == 3) {     /* a new triple formed         */
                if (((seq_t)43) < run_buf[0]
                 && ((seq_t)43) < run_buf[1]
                 && ((seq_t)43) < run_buf[2]) {
                    ensure_output_capacity(output_len + 3);
                    output_seq[output_len++] = run_buf[0];
                    output_seq[output_len++] = run_buf[1];
                    output_seq[output_len++] = run_buf[2];
                }
            }
        } else {                     /* run broken                 */
            run_buf[0] = (seq_t)p;
            run_len = 1;
        }
    }
    prev_prime = p;
}

/* ---------------------------------------------------------------------- */
/* generate primes by segmented sieve until we have at least n+1
   sequence elements, then return the n‑th element.                        */
seq_t a(idx_t n)
{
    if (!primes_inited) {
        init_small_primes();
        seg_high = seg_low + SEGMENT_SIZE;
    }

    /* generate until enough terms are ready */
    while (output_len <= (size_t)n) {
        size_t seg_size = (size_t)(seg_high - seg_low);
        unsigned char *segment = (unsigned char *)malloc(seg_size);
        memset(segment, 1, seg_size);           /* assume primes */

        /* sieve with pre‑computed small primes */
        for (size_t i = 0; i < small_prime_count; ++i) {
            uint32_t p = small_primes[i];
            uint64_t psq = (uint64_t)p * (uint64_t)p;
            if (psq > seg_high - 1ULL)
                break;
            uint64_t start = (seg_low + p - 1ULL) / (uint64_t)p * (uint64_t)p;
            if (start < psq)
                start = psq;
            for (uint64_t j = start; j < seg_high; j += (uint64_t)p)
                segment[(size_t)(j - seg_low)] = 0;
        }

        /* handle 0 and 1 if they appear in this segment */
        if (seg_low == 0) {
            if (seg_size > 0) segment[0] = 0;
            if (seg_size > 1) segment[1] = 0;
        } else if (seg_low == 1) {
            segment[0] = 0;
        }

        /* scan primes in the segment */
        for (uint64_t i = seg_low; i < seg_high; ++i)
            if (segment[(size_t)(i - seg_low)])
                process_prime(i);

        free(segment);

        seg_low  = seg_high;
        seg_high += SEGMENT_SIZE;
    }

    return output_seq[(size_t)n];
}
