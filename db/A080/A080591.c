/*
 * A080591  a(n) is taken to be the smallest positive integer greater than
 *          a(n-1) which is consistent with the condition "n is a member of
 *          the sequence if and only if a(n) is congruent to 3 mod 4".
 * https://oeis.org/A080591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A080591
   Definition: a(0)=1.  For n>0, a(n) is the smallest integer > a(n‑1)
   such that the functional equation a(a(n)) = 4n+3 holds.  Equivalently,
   n belongs to the sequence iff a(n) ≡ 3 (mod 4).  The values are
   produced by iterative construction. */

#include <stdbool.h>
#include <stdlib.h>

/* static state used for memoisation --------------------------------- */
static seq_t *A_arr     = NULL;   /* a(i) values                           */
static bool  *A_known   = NULL;   /* whether a(i) has been set             */
static size_t A_cap     = 0;      /* capacity of A_arr / A_known           */

static seq_t *forced    = NULL;   /* forced[v] = a(v) that must equal target*/
static bool  *forced_known = NULL;/* whether forced[v] has been set        */
static size_t forced_cap   = 0;    /* capacity of forced / forced_known     */

static void ensure_A(size_t idx)
{
    if (idx < A_cap) return;
    size_t newcap = A_cap ? A_cap * 2 : 128;
    while (newcap <= idx) newcap *= 2;
    A_arr   = (seq_t *)realloc(A_arr,   newcap * sizeof(seq_t));
    A_known = (bool  *)realloc(A_known, newcap * sizeof(bool));
    if (!A_arr || !A_known) abort();          /* allocation failure */
    for (size_t i = A_cap; i < newcap; ++i)    /* new bool slots were zero‑ed by realloc */
        A_known[i] = false;
    A_cap = newcap;
}

static void ensure_forced(size_t idx)
{
    if (idx < forced_cap) return;
    size_t newcap = forced_cap ? forced_cap * 2 : 128;
    while (newcap <= idx) newcap *= 2;
    forced        = (seq_t *)realloc(forced,        newcap * sizeof(seq_t));
    forced_known  = (bool  *)realloc(forced_known,  newcap * sizeof(bool));
    if (!forced || !forced_known) abort();        /* allocation failure */
    for (size_t i = forced_cap; i < newcap; ++i)
        forced_known[i] = false;
    forced_cap = newcap;
}

seq_t a(idx_t n)
{
    /* initialise static state on first call */
    if (!A_arr)     ensure_A(0);
    if (!forced)    ensure_forced(0);

    /* compute all values up to and including index n */
    for (idx_t i = 0; i <= n; ++i) {
        if (A_known[i]) continue;                 /* already known */

        seq_t val;
        if (forced_known[(size_t)i]) {
            /* value is forced by a previous step */
            val = forced[(size_t)i];
        } else {
            /* smallest integer greater than a(i‑1) */
            seq_t prev = (i == 0) ? (seq_t)0 : A_arr[i-1];
            val = prev + (seq_t)1;
        }

        /* store the computed value */
        A_arr[i]   = val;
        A_known[i] = true;

        /* enforce the functional relation a(a(i)) = 4*i + 3 */
        seq_t target = ((seq_t)4 * (seq_t)i) + (seq_t)3;
        size_t j = (size_t)val;
        ensure_forced(j);
        if (forced_known[j]) {
            if (forced[j] != target) abort();      /* inconsistency would mean a bug */
        } else {
            forced[j]        = target;
            forced_known[j]  = true;
        }
    }

    return A_arr[n];
}
