/*
 * A091436  Nonnegative integers whose name in Hebrew begins with the Hebrew
 *          letter "Aleph".
 * https://oeis.org/A091436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Return the nth nonnegative integer whose Hebrew
   name starts with the letter Aleph (א). */

#include <stdbool.h>

static const bool _unit_aleph[10] = {
    true,   /* 0: אפס          */
    true,   /* 1: אחד          */
    false,  /* 2: שניים        */
    false,  /* 3: שלשה         */
    true,   /* 4: ארבעה        */
    false,  /* 5: חמש          */
    false,  /* 6: שש           */
    false,  /* 7: שבע          */
    false,  /* 8: שמונה        */
    false   /* 9: תשעה         */
};

static const bool _tens_aleph[10] = {
    false,  /* 0  – unused                        */
    false,  /* 1  – unused for 10–19               */
    false,  /* 2  – 20 (עשרים)                   */
    false,  /* 3  – 30 (שלושים)                  */
    true,   /* 4  – 40 (ארבעים)                  */
    false,  /* 5  – 50 (חמשים)                   */
    false,  /* 6  – 60 (שישים)                   */
    false,  /* 7  – 70 (שבעים)                   */
    false,  /* 8  – 80 (שמונים)                  */
    false   /* 9  – 90 (תשעים)                   */
};

static const bool _hundreds_aleph[10] = {
    false,  /* 0  – unused                        */
    false,  /* 1  – 100 (מאה)                     */
    false,  /* 2  – 200 (שני מאות)                */
    false,  /* 3  – 300 (שלש מאות)               */
    true,   /* 4  – 400 (ארבע מאות)              */
    false,  /* 5  – 500 (חמש מאות)                */
    false,  /* 6  – 600 (שש מאות)                */
    false,  /* 7  – 700 (שבע מאות)                */
    false,  /* 8  – 800 (שמונה מאות)              */
    false   /* 9  – 900 (תשע מאות)               */
};

/* Return true if the Hebrew name of n starts with Aleph. */
static bool _aleph_first(idx_t n)
{
    if (n == 0)                     return true;           /* אפס */
    if (n < 10)                     return _unit_aleph[n];
    if (n < 20) {
        if (n == 10)                return false;          /* עשר */
        return _unit_aleph[n - 10];
    }
    if (n < 100)                    return _tens_aleph[n / 10];
    if (n < 1000)                   return _hundreds_aleph[n / 100];

    /* For n >= 1000, the first word comes from the thousands part. */
    idx_t part = n / 1000;
    if (part == 2)                  return true;             /* אלפיים */
    return _aleph_first(part);
}

seq_t a(idx_t n)
{
    idx_t pos   = 0;
    idx_t count = 0;

    while (true) {
        if (_aleph_first(pos)) {
            ++count;
            if (count == n)
                return (seq_t)pos;
        }
        ++pos;
    }
}
