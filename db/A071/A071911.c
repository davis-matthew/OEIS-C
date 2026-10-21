/*
 * A071911  Numbers m such that Stern's diatomic A002487(m) is divisible by 3.
 * https://oeis.org/A071911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A071911:  a(n) = the nth integer m (0‑based) for which
   Stern's diatomic sequence S(m) is divisible by 3.
   We compute S(m) modulo 3 on the fly using the recurrence
   S(0)=0, S(1)=1,
   S(2k)=S(k),   S(2k+1)=S(k)+S(k+1)  (mod 3).
   While scanning m upward we store the m's that satisfy S(m)≡0 (mod 3)
   into a static array and return the requested element.  This works
   for any build type of seq_t and idx_t. */

#include <stdlib.h>

static seq_t *terms = NULL;      /* stored values of the sequence */
static size_t terms_len  = 0;    /* number of terms currently stored */
static size_t terms_cap  = 0;    /* allocated capacity of terms */

static uint8_t *stern_mod3 = NULL;   /* Stern sequence values modulo 3 */
static size_t stern_len  = 0;         /* number of Stern values computed */
static size_t stern_cap  = 0;         /* allocated capacity of stern_mod3 */

static idx_t next_m = 0;     /* next m to evaluate */

/* Ensure the terms array has at least 'min_cap' capacity. */
static void grow_terms(size_t min_cap)
{
    if (min_cap <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms) abort();
    terms = new_terms;
    terms_cap = new_cap;
}

/* Ensure the stern_mod3 array has at least 'min_cap' capacity. */
static void grow_stern(size_t min_cap)
{
    if (min_cap <= stern_cap) return;
    size_t new_cap = stern_cap ? stern_cap * 2 : 128;
    while (new_cap < min_cap) new_cap <<= 1;
    uint8_t *new_stern = (uint8_t *)realloc(stern_mod3, new_cap * sizeof(uint8_t));
    if (!new_stern) abort();
    stern_mod3 = new_stern;
    stern_cap = new_cap;
}

/* Compute next terms until we have terms_len > n. */
static void generate_terms_up_to(idx_t n)
{
    while (terms_len <= n)
    {
        /* compute S(next_m) modulo 3 */
        uint8_t smod3;
        if (next_m == 0) smod3 = 0;
        else if (next_m == 1) smod3 = 1;
        else
        {
            if ((next_m & 1) == 0)        /* even */
            {
                smod3 = stern_mod3[next_m >> 1];
            }
            else                          /* odd */
            {
                uint8_t a = stern_mod3[(next_m - 1) >> 1];
                uint8_t b = stern_mod3[(next_m + 1) >> 1];
                smod3 = (a + b);
                if (smod3 >= 3) smod3 -= 3;   /* modulo 3 */
            }
        }

        grow_stern(next_m + 1);
        stern_mod3[next_m] = smod3;

        if (smod3 == 0)
        {
            seq_t val = (seq_t)next_m;
            grow_terms(terms_len + 1);
            terms[terms_len++] = val;
            /* we only need to store values up to terms_len-1 */
            /* But we must remember that we may stop before storing more if we already have enough terms. */
        }

        ++next_m;
    }
}

seq_t a(idx_t n)
{
    if (next_m == 0) {
        /* seed the Stern array with the base cases */
        grow_stern(2);
        stern_mod3[0] = 0;
        stern_mod3[1] = 1;
        terms_len = 0;
        terms_cap = 0;
        terms = NULL;
    }
    generate_terms_up_to(n);
    return terms[n];
}
