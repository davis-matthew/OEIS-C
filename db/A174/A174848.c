/*
 * A174848  Squarefree kernels of orders of sporadic simple groups.
 * https://oeis.org/A174848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = squarefree kernel of the order of the nth sporadic simple
 *            group, where the groups are ordered by increasing order
 * Range:     u8 none, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n < 1 || n > 26) return 0;   /* sequence has 26 terms only */

    /* String representations of the 26 radical values, in order. */
    static const char *const rad_str[26] = {
        "330",                 /* M11           */
        "330",                 /* M12           */
        "43890",               /* J1            */
        "2310",                /* M22           */
        "210",                 /* J2            */
        "53130",               /* M23           */
        "2310",                /* J3            */
        "9690",                /* HS            */
        "53130",               /* McL           */
        "2310",                /* He            */
        "3570",                /* Ru            */
        "79170",               /* Suz           */
        "30030",               /* G2(3)         */
        "1360590",             /* Fi22          */
        "53130",               /* Fi23          */
        "53130",               /* Fi24'         */
        "30030",               /* O'Nan         */
        "43890",               /* Ly            */
        "177521190",           /* Baby Monster  */
        "1607970",             /* Th            */
        "11741730",            /* Co3           */
        "690690",              /* Co3? (or Co1?)*/
        "75992317170",         /* Co2           */
        "340510170",           /* Co1           */
        "325046311590",        /* Baby Monster (addendum) */
        "1618964990108856390"  /* Monster       */
    };

    /* Convert strings to seq_t once and cache them. */
    static seq_t rad[26];
    static bool initialized = false;
    if (!initialized) {
        for (idx_t i = 0; i < 26; ++i)
            rad[i] = seq_from_str(rad_str[i]);
        initialized = true;
    }

    return rad[n - 1];
}
