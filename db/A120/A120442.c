/*
 * A120442  P-positions of John H. Conway's "Digit Deletions" game from "On
 *          Numbers and Games". Each number is the smallest positive integer
 *          that cannot be reduced to an earlier number in the sequence, by
 *          performing one of the following two operations: (1) changing one
 *          digit to a smaller digit (but not chang...
 * https://oeis.org/A120442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A120442 – compute the n‑th term of Conway’s Digit Deletions P‑positions
 *
 *  The sequence is generated incrementally:  a(1)=1.  For each
 *  successive integer x the algorithm checks whether it can be reduced
 *  to an earlier sequence element by one allowed operation:
 *      (1) change one digit to a smaller digit (leading zero forbidden)
 *      (2) delete a zero and all digits that follow it
 *
 *  If x cannot be reduced to an earlier element it is appended to the
 *  sequence.  This procedure reproduces the OEIS definition and works
 *  for all supported integer types.
 *
 *  The implementation keeps the sequence in a static array so calls
 *  in arbitrary order are efficient.  All arithmetic uses only
 *  integer operators, satisfying the required restrictions.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Helper: binary search – does array contain value?                 */
static bool seq_contains(const seq_t *seq, size_t count, seq_t value)
{
    size_t lo = 0, hi = count;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        seq_t v = seq[mid];
        if (v == value) return true;
        if (v < value) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/*  Helper: return decimal digits of x, most significant first.      */
/*  Caller must supply enough space (at least 20 digits for all test   */
/*  cases in this problem).                                            */
static size_t get_digits(seq_t x, uint8_t *buf, size_t maxlen)
{
    size_t pos = 0;
    seq_t tmp = x;
    /* Handle 0 explicitly – not needed here but keeps function robust */
    if (tmp == (seq_t)0) {
        if (maxlen < 1) return 0;
        buf[0] = 0;
        return 1;
    }
    while (tmp > (seq_t)0 && pos < maxlen) {
        uint8_t d = (uint8_t)(tmp % (seq_t)10);
        buf[pos++] = d;
        tmp /= (seq_t)10;
    }
    /* reverse into most‑significant‑first order */
    for (size_t i = 0; i < pos / 2; ++i) {
        uint8_t t = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = t;
    }
    return pos;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Persistent state across calls */
    static seq_t *terms     = NULL;  /* stored sequence values      */
    static size_t terms_cap = 0;     /* allocated capacity          */
    static size_t terms_cnt = 0;     /* number of values stored     */
    static seq_t next_candidate = (seq_t)1; /* next integer to examine */

    /* Initialize storage on first call */
    if (!terms) {
        terms_cap = 1024;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) return (seq_t)0;          /* allocation failed */
    }

    /* Generate terms until we have at least n of them */
    while (terms_cnt < (size_t)n) {
        seq_t cand = next_candidate;
        bool reducible = false;

        /* Extract digits of cand (max 256 digits – more than enough) */
        uint8_t digits[256];
        size_t dlen = get_digits(cand, digits, sizeof(digits));
        if (dlen == 0) dlen = 1;   /* safety, should not happen */

        /* -------- Operation (1) – change one digit to a smaller one -------- */
        for (size_t pos = 0; pos < dlen && !reducible; ++pos) {
            uint8_t orig = digits[pos];
            for (uint8_t newd = 0; newd < orig; ++newd) {
                if (pos == 0 && newd == 0) continue;   /* leading zero forbidden */

                seq_t val = 0;
                for (size_t i = 0; i < dlen; ++i) {
                    uint8_t d = (i == pos) ? newd : digits[i];
                    val = val * (seq_t)10 + (seq_t)d;
                }
                if (seq_contains(terms, terms_cnt, val)) {
                    reducible = true;
                    break;
                }
            }
        }

        /* -------- Operation (2) – delete a zero and all following digits -------- */
        for (size_t pos = 0; pos < dlen && !reducible; ++pos) {
            if (digits[pos] != 0) continue;
            /* Keep digits before the zero (exclusive) */
            seq_t val = 0;
            for (size_t i = 0; i < pos; ++i) {
                val = val * (seq_t)10 + (seq_t)digits[i];
            }
            if (seq_contains(terms, terms_cnt, val)) {
                reducible = true;
                break;
            }
        }

        /* If not reducible, append to the sequence */
        if (!reducible) {
            if (terms_cnt == terms_cap) {
                terms_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;          /* allocation failed */
                terms = tmp;
            }
            terms[terms_cnt++] = cand;
        }

        /* Move to next integer for the next iteration */
        next_candidate++;
    }

    /* Return the requested term (1‑based index) */
    return terms[n - 1];
}
